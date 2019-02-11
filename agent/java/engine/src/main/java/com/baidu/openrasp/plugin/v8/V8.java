package com.baidu.openrasp.plugin.v8;

import com.jsoniter.spi.JsoniterSpi;
import com.jsoniter.extra.Base64Support;
import com.jsoniter.output.JsonStream;
import com.jsoniter.JsonIterator;
import org.apache.log4j.Logger;
import java.io.File;
import java.io.FileFilter;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import com.baidu.openrasp.HookHandler;
import com.baidu.openrasp.config.Config;
import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.FileUtils;
import com.baidu.openrasp.cloud.model.ErrorType;
import com.baidu.openrasp.cloud.utils.CloudUtils;
import com.baidu.openrasp.plugin.info.AttackInfo;
import com.baidu.openrasp.plugin.info.EventInfo;
import com.baidu.openrasp.plugin.v8.Result;
import com.baidu.openrasp.request.AbstractRequest;
import com.baidu.openrasp.plugin.checker.CheckParameter;
import com.baidu.openrasp.plugin.checker.CheckParameter.Type;

public class V8 {
    private static final Logger PLUGIN_LOGGER = Logger.getLogger(V8.class.getPackage().getName() + ".log");
    private static final Logger LOGGER = Logger.getLogger(V8.class.getPackage().getName());
    private static ThreadLocal<ArrayList<String>> stringInfos = new ThreadLocal<ArrayList<String>>() {
        @Override
        protected ArrayList<String> initialValue() {
            return null;
        }
    };

    static {
        try {
            System.load(
                    "/Users/lanyuhang/Workspace/openrasp/agent/java/engine/src/main/native/build/libopenrasp_js_engine.dylib");
        } catch (UnsatisfiedLinkError e) {
            System.load("/tmp/libopenrasp_js_engine.so");
        }
        JsoniterSpi.setDefaultConfig(new com.jsoniter.spi.Config.Builder().escapeUnicode(false).build());
        Base64Support.enable();
    }

    public static byte[] ttttt(int[] a) {
        byte[] ret = new byte[] { 66, 67, 68 };
        a[0] = 3;
        return ret;
    }

    public synchronized static native boolean Initialize();

    public synchronized static native boolean Dispose();

    public synchronized static native boolean CreateSnapshot(String config, Object[] plugins);

    public static native boolean Check(String type, byte[] params, int params_size, byte[] context, int context_size);

    public static native boolean Check(String type, byte[] params, int params_size, Context context,
            boolean new_request);

    public static void PluginLog(String log) {
        PLUGIN_LOGGER.info(log.replaceAll("\n$", ""));
    }

    public static void AlarmLog(String log) {
        ArrayList<String> infos = stringInfos.get();
        if (infos == null) {
            infos = new ArrayList<String>();
            stringInfos.set(infos);
        }
        infos.add(log);
    }

    public static List<EventInfo> Check(CheckParameter checkParameter) {
        Type type = checkParameter.getType();
        V8ByteArrayOutputStream params = new V8ByteArrayOutputStream();
        JsonStream.serialize(checkParameter.getParams(), params);

        boolean isCheckLru = type == Type.DIRECTORY || type == Type.READFILE || type == Type.WRITEFILE || type == Type.SQL;
        if (isCheckLru) {
            if (HookHandler.commonLRUCache.isContainsKey(ByteBuffer.wrap(params.getByteArray()))) {
                return null;
            }
        }

        stringInfos.set(null);
        boolean isBlock = V8.Check(type.getName(), params.getByteArray(), params.size(), new Context(checkParameter.getRequest()),
                type == Type.REQUEST);
        ArrayList<String> infos = stringInfos.get();
        stringInfos.set(null);
        ArrayList<EventInfo> attackInfos = null;
        if (infos != null) {
            attackInfos = new ArrayList<EventInfo>();
            for (int i = 0; i < infos.size(); i++) {
                try {
                    Result result = JsonIterator.deserialize(infos.get(i), Result.class);
                    AttackInfo info = new AttackInfo(checkParameter, result.action, result.message, result.name,
                            result.algorithm, result.confidence);
                    attackInfos.add(info);
                } catch (Exception e) {
                    LOGGER.error(e);
                }
            }
        }

        if (!isBlock && isCheckLru) {
            if (HookHandler.commonLRUCache.maxSize() != 0) {
                HookHandler.commonLRUCache.put(ByteBuffer.wrap(params.getByteArray()), null);
            }
        }

        return attackInfos;
    }

    public synchronized static void UpdatePlugin() throws Exception {
        boolean oldValue = HookHandler.enableHook.getAndSet(false);
        List<String[]> scripts = new ArrayList<String[]>();
        File pluginDir = new File(Config.getConfig().getScriptDirectory());
        LOGGER.debug("checker directory: " + pluginDir.getAbsolutePath());
        if (!pluginDir.isDirectory()) {
            pluginDir.mkdir();
        }
        FileFilter filter = FileFilterUtils.and(FileFilterUtils.sizeFileFilter(10 * 1024 * 1024, false),
                FileFilterUtils.suffixFileFilter(".js"));
        File[] pluginFiles = pluginDir.listFiles(filter);
        if (pluginFiles != null) {
            for (File file : pluginFiles) {
                try {
                    String name = file.getName();
                    String source = FileUtils.readFileToString(file, "UTF-8");
                    scripts.add(new String[] { name, source });
                } catch (Exception e) {
                    String message = e.getMessage();
                    int errorCode = ErrorType.PLUGIN_ERROR.getCode();
                    LOGGER.error(CloudUtils.getExceptionObject(message, errorCode), e);
                }
            }
        }
        CreateSnapshot("{}", scripts.toArray());
        HookHandler.enableHook.set(oldValue);
    }

    public static void main(String[] args) {

    }
}
