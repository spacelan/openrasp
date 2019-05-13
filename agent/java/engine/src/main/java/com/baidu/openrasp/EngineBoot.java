/*
 * Copyright 2017-2019 Baidu Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.baidu.openrasp;

import com.baidu.openrasp.cloud.utils.CloudUtils;
import com.baidu.openrasp.messaging.LogConfig;
import com.baidu.openrasp.plugin.checker.CheckerManager;
import com.baidu.openrasp.tool.model.BuildRASPModel;
import com.baidu.openrasp.transformer.CustomClassTransformer;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.lang.instrument.Instrumentation;
import java.lang.instrument.UnmodifiableClassException;
import java.net.URL;
import java.util.jar.Attributes;
import java.util.jar.Manifest;

import com.baidu.openrasp.plugin.js.*;
/**
 * Created by tyy on 18-1-24.
 *
 * OpenRasp 引擎启动类
 */
public class EngineBoot implements Module {

    private String projectVersion;
    private String buildTime;
    private String gitCommit;
    private CustomClassTransformer transformer;

    @Override
    public void start(String mode, Instrumentation inst) throws Exception {
        System.out.println("\n\n" +
                "   ____                   ____  ___   _____ ____ \n" +
                "  / __ \\____  ___  ____  / __ \\/   | / ___// __ \\\n" +
                " / / / / __ \\/ _ \\/ __ \\/ /_/ / /| | \\__ \\/ /_/ /\n" +
                "/ /_/ / /_/ /  __/ / / / _, _/ ___ |___/ / ____/ \n" +
                "\\____/ .___/\\___/_/ /_/_/ |_/_/  |_/____/_/      \n" +
                "    /_/                                          \n\n");
        if (!loadConfig()) {
            return;
        }
        readVersion();
        // 初始化插件系统
        JS.Initialize();
        CheckerManager.init();
        initTransformer(inst);
        String message = "OpenRASP Engine Initialized [" + projectVersion + " (build: GitCommit=" + gitCommit + " date="
                + buildTime + ")]";
        System.out.println(message);
        Logger.getLogger(EngineBoot.class.getName()).info(message);
    }

    @Override
    public void release(String mode) {
        if (transformer != null) {
            transformer.release();
        }
        JS.Dispose();
        CheckerManager.release();
        String message = "OpenRASP Engine Released [" + projectVersion + " (build: GitCommit=" + gitCommit + " date="
                + buildTime + ")]";
        System.out.println(message);
    }

    /**
     * 初始化配置
     *
     * @return 配置是否成功
     */
    private boolean loadConfig() throws Exception {
        LogConfig.ConfigFileAppender();
        //单机模式下动态添加获取删除syslog
        if (!CloudUtils.checkCloudControlEnter()) {
            LogConfig.syslogManager();
        }
        return true;
    }

    /**
     * 初始化类字节码的转换器
     *
     * @param inst 用于管理字节码转换器
     */
    private void initTransformer(Instrumentation inst) throws UnmodifiableClassException {
        transformer = new CustomClassTransformer(inst);
        transformer.retransform();
    }

    private void readVersion() throws IOException {
        Class clazz = EngineBoot.class;
        String className = clazz.getSimpleName() + ".class";
        String classPath = clazz.getResource(className).toString();
        String manifestPath = classPath.substring(0, classPath.lastIndexOf("!") + 1) + "/META-INF/MANIFEST.MF";
        Manifest manifest = new Manifest(new URL(manifestPath).openStream());
        Attributes attr = manifest.getMainAttributes();
        projectVersion = attr.getValue("Project-Version");
        buildTime = attr.getValue("Build-Time");
        gitCommit = attr.getValue("Git-Commit");

        projectVersion = (projectVersion == null ? "UNKNOWN" : projectVersion);
        buildTime = (buildTime == null ? "UNKNOWN" : buildTime);
        gitCommit = (gitCommit == null ? "UNKNOWN" : gitCommit);
        //缓存rasp的build信息
        BuildRASPModel.initRaspInfo(projectVersion, buildTime, gitCommit);
    }

}
