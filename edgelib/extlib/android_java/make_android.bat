@echo off
@echo cleaning
del nl\elements\edgelib\*.class
del nl\elements\edgelib\*.h
@echo building
"c:\Program Files\Java\jdk1.6.0_25\bin\javac.exe" -classpath . -bootclasspath c:\androidsdk\platforms\android-3\android.jar -target 1.5 -encoding UTF-8 -source 1.5 nl\elements\edgelib\EdgelibApp.java
"c:\Program Files\Java\jdk1.6.0_25\bin\javah.exe" -classpath . -o nl\elements\edgelib\EdgelibApp.h nl.elements.edgelib.EdgelibApp 
"c:\Program Files\Java\jdk1.6.0_25\bin\javac.exe" -classpath . -bootclasspath c:\androidsdk\platforms\android-3\android.jar -target 1.5 -encoding UTF-8 -source 1.5 nl\elements\edgelib\EdgelibView.java
"c:\Program Files\Java\jdk1.6.0_25\bin\javah.exe" -classpath . -o nl\elements\edgelib\EdgelibView.h nl.elements.edgelib.EdgelibView
"c:\Program Files\Java\jdk1.6.0_25\bin\javac.exe" -classpath . -bootclasspath c:\androidsdk\platforms\android-3\android.jar -target 1.5 -encoding UTF-8 -source 1.5 nl\elements\edgelib\Edgelib3DView.java
"c:\Program Files\Java\jdk1.6.0_25\bin\javah.exe" -classpath . -o nl\elements\edgelib\Edgelib3DView.h nl.elements.edgelib.Edgelib3DView
"c:\Program Files\Java\jdk1.6.0_25\bin\javac.exe" -classpath . -bootclasspath c:\androidsdk\platforms\android-3\android.jar -target 1.5 -encoding UTF-8 -source 1.5 nl\elements\edgelib\EdgelibUtil.java
"c:\Program Files\Java\jdk1.6.0_25\bin\javah.exe" -classpath . -o nl\elements\edgelib\EdgelibUtil.h nl.elements.edgelib.EdgelibUtil
"c:\Program Files\Java\jdk1.6.0_25\bin\javac.exe" -classpath . -bootclasspath c:\androidsdk\platforms\android-3\android.jar -target 1.5 -encoding UTF-8 -source 1.5 nl\elements\edgelib\EdgelibFile.java
"c:\Program Files\Java\jdk1.6.0_25\bin\javah.exe" -classpath . -o nl\elements\edgelib\EdgelibFile.h nl.elements.edgelib.EdgelibFile
"c:\Program Files\Java\jdk1.6.0_25\bin\jar.exe" cvf edge.jar nl\elements\edgelib\*.class
@echo installing
copy /Y edge.jar ..\..\..\dist\lib\android\