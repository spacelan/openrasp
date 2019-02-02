#!/bin/bash

pushd ../java
javah com.baidu.openrasp.plugin.v8.V8
mv com_baidu_openrasp_plugin_v8_V8.h ../native
popd