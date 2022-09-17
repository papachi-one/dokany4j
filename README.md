# dokany4j
Java library with JNI native library bindings for Dokany 2.0.x https://github.com/dokan-dev/dokany
## How to build
### JNI library (dokany4j.dll)
- Install Visual Studio 2022
    - You can download it from [here](https://visualstudio.microsoft.com/downloads/).
- Install CMake
    - You can download it from [here](https://cmake.org/download/).
- Install Dokany 2.x.x
  - You can download it from [here](https://github.com/dokan-dev/dokany/releases).
  - During installation choose "Development" option.
- Open command prompt in the project directory and run the following commands:
    - `cmake -H.\native -B.\native\build`
    - `cmake --build .\native\build --config Release`
    - `copy /y .\native\build\Release\dokany4j.dll .\src\main\resources\one\papachi\dokany4j`
### Java library (dokany4j-x.y.z.jar)
- Install JDK 17
  - You can download it from [here](https://www.oracle.com/java/technologies/downloads/).
- Install maven
  - You can download it from [here](https://maven.apache.org/download.cgi).
- Open command prompt in the project directory and run the following commands:
  - `mvn install`
## How to use
If you are using maven as your build tool, you can reference the library in your `pom.xml`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>one.papachi</groupId>
  <artifactId>example</artifactId>
  <version>1.0.0</version>

  <properties>
    <maven.compiler.source>17</maven.compiler.source>
    <maven.compiler.target>17</maven.compiler.target>
  </properties>

  <dependencies>
    <dependency>
      <groupId>one.papachi</groupId>
      <artifactId>dokany4j</artifactId>
      <version>1.0.0</version>
    </dependency>
  </dependencies>

</project>
```