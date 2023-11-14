REM
REM Keenan's Hack to remove intermediate files
REM Designed to work for C++ or C# visual studio projects
REM
REM If you find a way to enhance, 
REM please return updates to ekeenan2@cdm.depaul.edu
REM

REM cleanup root director

REM cleanup root director

    rmdir /S /Q Logs
    rmdir /S /Q Debug
    rmdir /S /Q Release
    rmdir /S /Q x64
    rmdir /S /Q ipch
    rmdir /S /Q .vs
    del /F /Q Engine\Shaders\Compiled\*.*

    del /F /S /A:H *.suo
    del /F /S *.sdf
    del /F /S *.suo
    del /F *.db
  
REM walk through subdirectories delete Release & Debug subs

for /d %%D in (*) do (
	echo %%~fD
	del /F %%~D\*.bin
	rmdir /S /Q %%~D\Debug
	rmdir /S /Q %%~D\Release
	rmdir /S /Q %%~D\x64

)

REM walk through subdirectories delete generated subdirectories

for /d %%D in (*) do (
	echo %%~fD
	cd %%~D
	CleanMe.bat
	cd ..
)