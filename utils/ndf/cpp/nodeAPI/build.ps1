Write-Output "------------------------- ����CMakeĿ�� --------------------------"
cmake -B build -S .

Write-Output "`n`n------------------------- ����Ŀ�� -------------------------------"
cmake --build build --config Release

Write-Output "`n`n------------------------- �ƶ�Ŀ���ļ� ----------------------------"
Move-Item -Path ./build/Release/ndfNodeAPI.node -Destination ./node -Force

Write-Output "`n`n------------------------- ���в��� -------------------------------"
node ./node/test.js

Write-Output "�����ѽ��� �˳���: $LastExitCode"
