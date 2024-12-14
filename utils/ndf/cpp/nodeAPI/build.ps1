Write-Output "------------------------- 加载CMake目标 --------------------------"
cmake -B build -S .

Write-Output "`n`n------------------------- 编译目标 -------------------------------"
cmake --build build --config Release

Write-Output "`n`n------------------------- 移动目标文件 ----------------------------"
Move-Item -Path ./build/Release/ndfNodeAPI.node -Destination ./node -Force

Write-Output "`n`n------------------------- 运行测试 -------------------------------"
node ./node/test.js

Write-Output "进程已结束 退出码: $LastExitCode"
