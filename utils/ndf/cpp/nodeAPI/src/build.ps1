Write-Output "------------------------- ����CMakeĿ�� --------------------------"
cmake -B build -S .

Write-Output "`n`n------------------------- ����Ŀ�� -------------------------------"
cmake --build build --config Release

Write-Output "`n`n------------------------- �ƶ�Ŀ���ļ� ----------------------------"
Move-Item -Path ./build/Release/ndfNodeAPI.node -Destination ../ndfNodeAPI/src/ -Force

Write-Output "`n`n------------------------- ���в��� -------------------------------"
ts-node ../test/test.ts

Write-Output "�����ѽ��� �˳���: $LastExitCode"
