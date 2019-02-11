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

package com.baidu.openrasp.hook.file;

import com.baidu.openrasp.HookHandler;
import com.baidu.openrasp.hook.AbstractClassHook;
import com.baidu.openrasp.plugin.checker.CheckParameter;
import com.baidu.openrasp.tool.Reflection;
import com.baidu.openrasp.tool.annotation.HookAnnotation;
import javassist.CannotCompileException;
import javassist.CtClass;
import javassist.NotFoundException;
import java.util.HashMap;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;

/**
 * Created by zhuming01 on 5/5/17.
 * All rights reserved
 */
@HookAnnotation
public class DiskFileItemHook extends AbstractClassHook {
    /**
     * (none-javadoc)
     *
     * @see com.baidu.openrasp.hook.AbstractClassHook#getType()
     */
    @Override
    public String getType() {
        return "fileUpload";
    }


    /**
     * (none-javadoc)
     *
     * @see com.baidu.openrasp.hook.AbstractClassHook#isClassMatched(String)
     */
    @Override
    public boolean isClassMatched(String className) {
        return "org/apache/commons/fileupload/disk/DiskFileItem".equals(className);
    }

    @Override
    protected void hookMethod(CtClass ctClass) throws IOException, CannotCompileException, NotFoundException {
        String src = getInvokeStaticSrc(DiskFileItemHook.class, "checkFileUpload",
                "getName(),get(),$1", String.class, byte[].class, Object.class);
        insertAfter(ctClass, "setHeaders", null, src, true);
    }

    /**
     * 文件上传hook点
     *
     * @param name    文件名
     * @param content 文件数据
     */
    public static void checkFileUpload(String name, byte[] content, Object object) {
        if (name != null && content != null && object != null) {
            HashMap<String, Object> params = new HashMap<String, Object>();
            params.put("filename", name);
            if (content.length > 4 * 1024) {
                content = Arrays.copyOf(content, 4 * 1024);
            }
            params.put("content", content);
            String customFileName = Reflection.invokeStringMethod(object, "getHeader", new Class[]{String.class}, "content-disposition");
            if (customFileName != null) {
                customFileName = getFileName(customFileName);
            }
            params.put("name", customFileName != null ? customFileName : "");

            HookHandler.doCheck(CheckParameter.Type.FILEUPLOAD, params);
        }
    }

    private static String getFileName(String name) {
        String[] fields = name.split(";");
        for (String field : fields) {
            if (!field.contains("filename") && field.contains("name")) {
                int index = field.indexOf("=");
                if (index > 0 && index < field.length() - 1) {
                    return field.substring(index + 1);
                }
            }
        }
        return null;
    }
}
