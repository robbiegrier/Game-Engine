REM
REM Keenan's Hack to remove intermediate files
REM If you find a way to enhance, please return updates to
REM ekeenan2@cdm.depaul.edu
REM

REM cleanup root director

    rmdir /S /Q Logs
    rmdir /S /Q Debug
    rmdir /S /Q Release
    rmdir /S /Q x64
    rmdir /S /Q ipch
    rmdir /S /Q .vs

    del /F /S /A:H *.suo
    del /F /S *.sdf
    del /F /S *.suo
    del /F *.db

	del /F *.bin
        del /F .\..\data\*.azul
        del /F .\..\data\*.tga
        del /F *.pb.*
	rmdir /S /Q Debug
	rmdir /S /Q Release
	rmdir /S /Q x64
