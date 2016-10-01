echo off

pushd "%cd%\.."


set DATA_RAW=DataRaw\Data
set DATA_COPY_TO_ARK=DataPacked\Ark\customdata\Data
set DATA_COPY_TO=DataPacked\Data
set DATA_PACK=DataPacked\DataPack

echo ================copy data ==================:

xcopy /s/y %DATA_RAW%\Texture 				 %DATA_COPY_TO_ARK%\Texture

xcopy /s/y %DATA_RAW%\fonts 				 %DATA_COPY_TO%\Data\fonts

xcopy /s/y %DATA_RAW%\xml 					 %DATA_COPY_TO_ARK%\xml
xcopy /s/y %DATA_RAW%\Material 				 %DATA_COPY_TO_ARK%\Material

xcopy /s/y %DATA_RAW%\Sound 				 %DATA_COPY_TO%%\Data\Sound
xcopy /s/y %DATA_RAW%\loading.png 			 %DATA_COPY_TO%\Data
xcopy /s/y %DATA_RAW%\loading_ipad.png 		 %DATA_COPY_TO%\Data
xcopy /s/y %DATA_RAW%\Music 				 %DATA_COPY_TO%\Data\Music



echo ================make ark ==================:

call DataPacked\Ark\CreateArk.cmd

xcopy /s/y %DATA_COPY_TO_ARK%\..\..\customdata.archived\*.ark 	%DATA_COPY_TO%\

popd