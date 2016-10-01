echo off

pushd "%cd%\.."


set DATA_RAW=Resources
set DATA_COPY_TO_W8=MainProject\Win8.1\Debug\MainGameProj.Windows\AppX\Data

echo ================copy data ==================:

xcopy /s/y %DATA_RAW%\Texture\atlast_texture %DATA_COPY_TO_W8%\Texture
xcopy /s/y %DATA_RAW%\fonts 				 %DATA_COPY_TO_W8%\fonts
xcopy /s/y %DATA_RAW%\xml 					 %DATA_COPY_TO_W8%\xml
xcopy /s/y %DATA_RAW%\Material 				 %DATA_COPY_TO_W8%\Material
xcopy /s/y %DATA_RAW%\Sound 				 %DATA_COPY_TO_W8%\Sound
xcopy /s/y %DATA_RAW%\Music 				 %DATA_COPY_TO_W8%\Music
xcopy /s/y %DATA_RAW%\loading.png 				 %DATA_COPY_TO_W8%\
popd