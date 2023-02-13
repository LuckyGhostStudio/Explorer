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

### 1. 项目入口点

- EntryPoint.h
- Application.h/.cpp
- SandBoxApp.cpp

### 2. 日志系统

- Log.h/.cpp

### 3. Premake构建项目

- premake.lua

### 4. 实现事件系统

- Event.h
- ApplicationEvent.h
- KeyEvent.h
- MouseEvent.h

### 5. 创建预编译头文件

- exppch.h/.cpp

### 6. 窗口抽象和GLFW

- Window.h/.cpp

### 7. 窗口事件

### 8. Layer

- Layer.h/.cpp
- LayerStack.h/.cpp

### 9. GLAD

### 10.ImGui层

- ImGuiLayer.h/.cpp

### 11. ImGui事件

### 12. 输入轮询

- Input.h/.cpp

### 13. 按键和鼠标代码

- KeyCodes.h
- MouseButtonKeyCodes.h

### 14. glm数学库

### 15. ImGui的docking

- ImGuiBuild.h

## 文件说明

### Explorer 项目

#### src

##### 预编译头文件

- exppch.h/.cpp。

  包含引擎需要用到的C++标准库文件和其他库文件，预先编译为二进制文件，加快项目构建速度。

##### 外部文件

- spdlog

  外部日志库文件。

- GLFW

  外部窗口库文件，配合OpenGL。

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

  引擎应用程序入口，应用基类。定义所有事件处理函数，创建窗口并为窗口设置事件函数，接收事件并进行事件调度，将事件传递给层。创建ImGui层。定义引擎循环，更新层栈中的所有层，渲染GUI，更新窗口。

- EntryPoint.h

  引擎入口点，负责创建应用程序。

-  Log.h/.cpp

  引擎日志系统，定义内核日志和客户端日志基本格式。

- Explorer.h

  引用Explorer.dll需要包含的所有头文件。
  
- Input.h/.cpp

  输入类，定义实现按键鼠标输入接口。

##### 窗口文件

- Window.h

  窗口类，可创建事件。

##### Layer文件

- Layer.h/.cpp

  所有层的基类，可接收事件并处理。

- LayerStack.h/.cpp

  存储所有层，普通层在列表前半部分，覆盖层在列表后半部分。
  
- ImGuiLayer.h/.cpp

  ImGui层，渲染ImGui。

### Sandbox 项目

#### src

- SandboxApp.cpp

  应用程序类，继承Application。可创建具体Layer。

## 子系统

### 日志系统

### 事件系统

### 输入系统

