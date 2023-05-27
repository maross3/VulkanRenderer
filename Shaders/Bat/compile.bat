for %%F in (".\Shaders\*.vert") do C:\VulkanSDK\1.3.216.0\Bin\glslc.exe .\Shaders\%%~nxF -o .\Shaders\%%~nxF.spv
for %%F in (".\Shaders\*.frag") do C:\VulkanSDK\1.3.216.0\Bin\glslc.exe .\Shaders\%%~nxF -o .\Shaders\%%~nxF.spv

pause