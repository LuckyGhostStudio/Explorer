# Explorer-Engine

Explorer Engine This is a simple Rendering Engine based on OpenGL and C++.

## 项目结构

### Explorer

- 引擎核心
- 编译后生成静态库文件和动态链接库（.dll）文件
- 在运行时链接到Sandbox
- 由Sandbox调用

### Sandbox

- 引擎应用启动项
- 最终编译为exe文件

## 开发过程

### 项目入口点

- EntryPoint.h

### 日志系统

- Log.h
- Log.cpp

### Premake构建项目

- premake.lua

### 实现事件系统

- Event.h
- ApplicationEvent.h
- KeyEvent.h
- MouseEvent.h

## 文件说明

### Explorer 项目

#### src

##### Events

* Event.h

  定义所有事件基类 Event、事件调度器 EventDispatcher，定义事件类型 EventType 和事件种类 EventCategory。

* ApplicationEvent.h

  定义所有窗口事件、App事件。

* KeyEvent.h

  定义所有键盘事件 KeyEvent。

* MouseEvent.h

  定义所有鼠标事件。

##### 内核文件

- Core.h

  引擎内核文件，定义引擎需要的宏和条件编译。

- Application.h/.cpp

  引擎应用程序入口，应用基类。

- EntryPoint.h

  引擎入口点，负责创建应用程序。

-  Log.h/.cpp

  引擎日志系统，定义内核日志和客户端日志基本格式

- Explorer.h

  引用Explorer.dll需要包含的所有头文件

##### 外部文件

- spdlog

  外部日志库

### Sandbox 项目

#### src

- SandboxApp.cpp

  应用程序类，继承Application

## 子系统

### 日志系统

### 事件系统

![](E:\毕业设计\Explorer\事件系统.png)

