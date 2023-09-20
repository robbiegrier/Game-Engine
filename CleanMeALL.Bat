REM
REM Keenan's Hack to remove intermediate files
REM Designed to work for C++ or C# visual studio projects
REM
REM If you find a way to enhance, 
REM please return updates to ekeenan2@cdm.depaul.edu
REM

REM cleanup root director

CleanMe.bat


REM walk through subdirectories delete generated subdirectories

for /d %%D in (*) do (
	echo %%~fD
	cd %%~D
	CleanMe.bat
	cd ..
)