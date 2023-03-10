# Explorer-Engine

Explorer Engine This is a simple Rendering Engine based on OpenGL and C++.

## 项目结构

### Explorer

- 引擎核心
- 编译后生成静态库文件
- 链接到Sandbox
- 由Sandbox调用

### ExplorerEditor

- 编辑器
- exe文件
- 构建应用程序

### Sandbox

- 测试项目
- exe文件
- 测试应用程序

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

### 16. 静态库和零警告

### 17. 渲染上下文

- OpenGLContext.h/.cpp

### 18. 第一个三角形

### 19. 着色器

- Shader.h/.cpp

### 20. 渲染器接口抽象

- Buffer.h/.cpp
- Renderer.h/.cpp

### 21. 顶点缓冲区布局

### 22. 顶点数组

- VertexArray.h/.cpp

### 23. 渲染流和渲染命令

- RenderCommand.h/.cpp

### 24. 相机

- Camera.h/.cpp

### 25. 移动渲染示例到SandBox

### 26. 帧间隔DeltaTime

- DeltaTime.h

### 27. Transforms

### 28. 纹理

- Texture.h/.cpp

### 29. 颜色混合

### 30. 着色器资产文件

### 31. 着色器库

### 32. 相机控制器

- CameraController.h/.cpp

### 33. 窗口缩放调整

### 34. 准备2D渲染

- Sandbox2D.h/.cpp

### 35. 开始2D渲染

- Renderer2D.h/.cpp

### 36. 2D渲染Transform

### 37. 2D渲染Texture

### 38. 基本性能测试

### 39. 批渲染

### 40. 批渲染数据统计

### 41. 创建子纹理

- SubTexture2D.h/.cpp

### 42. DockSpace

### 43.帧缓冲区

- Framebuffer.h/.cpp

### 44. 场景视口

### 45. ImGuiLayer事件

### 46. 组件系统

- Scene.h/.cpp
- Components.h

### 47. 实体

- Object.h/.cpp

### 48. 本地脚本

- ScriptableObject.h

49.场景Hierarchy面板

- SceneHierarchyPanel.h/.cpp

### 50. 属性面板

### 51. 相机组件

### 52. Sprite Renderer组件UI

### 53. Transform组件UI

### 54. 添加、移除实体和组件UI

### 55. 优化UI样式和布局

### 56. 序列化和反序列化场景

### 57. 打开和保存场景文件

### 58. Transform Gizmo

### 59. EditorCamera

### 60. 多渲染目标缓冲区

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

##### Core

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
  
- DeltaTime.h

  帧间隔，计算每帧之间的时间间隔。
  
- Window.h

  窗口类，可创建事件。

##### Layer文件

- Layer.h/.cpp

  所有层的基类，可接收事件并处理。

- LayerStack.h/.cpp

  存储所有层，普通层在列表前半部分，覆盖层在列表后半部分。
  
- ImGuiLayer.h/.cpp

  ImGui层，渲染ImGui。

##### Math

- Math.h/.cpp

  数学库，包括分解transform矩阵函数，将transform矩阵分解为position、rotation和scale向量。

##### Renderer

- OpenGLContext.h/.cpp

  OpenGL上下文类，初始化OpenGL上下文，交换双缓冲。
  
- Shader.h/.cpp

  着色器，顶点着色器处理顶点，片元着色器处理像素，最终输出该像素的正确颜色。
  
- Renderer.h/.cpp

  渲染器，设置场景参数，进行渲染命令提交。

- Renderer2D.h/.cpp

  2D渲染器，进行2D图形渲染。

- Buffer.h/.cpp

  缓冲类，包含顶点缓冲和索引缓冲。缓冲区布局接口，格式化缓冲区布局。
  
- VertexArray.h/.cpp

  顶点数组类，链接多个顶点缓冲区和索引缓冲区，整合缓冲区布局。
  
- RenderCommand.h/.cpp

  渲染命令，执行最底层的OpenGL渲染命令。
  
- Camera.h/.cpp

  相机，进行相机变换，包括计算投影矩阵、视图矩阵，进行视图变换和投影变换。
  
- Texture.h/.cpp

  纹理类，进行纹理加载和纹理格式设置。
  
- SubTexture2D.h/.cpp

  子纹理，根据子纹理坐标从Texture中裁剪。

- CameraController.h/.cpp

  相机控制器，控制相机移动、旋转，窗口缩放。
  
- Framebuffer.h/.cpp

  帧缓冲区，可创建帧缓冲区，包括颜色缓冲区和深度缓冲区，可将颜色缓冲区显示在ImGui上。
  
- EditorCamera.h/.cpp

  编辑器相机，编辑器内场景渲染相机，可自由移动，用来查看场景，不同于相机组件。

##### Scene

- Scene.h/.cpp

  场景，包括所有的实体对象。

- Components.h

  组件，所有组件。
  
- Object.h/.cpp

  实体，场景中的物体对象，保存物体id，和其他参数，通过id关联所有组件。
  
- ScriptableObject.h

  可脚本化对象，所有本地脚本的基类。
  
- SceneSerializer.h/.cpp

  场景序列化器，可序列化和反序列化场景。
  
- PlatformUtils.h/.cpp

  平台工具，包括文件对话框，可打开打开文件和保存文件对话框。

### Editor 项目

#### asserts

- ##### shaders

  着色器文件，包括顶点着色器和片元着色器。

- ##### textures

  贴图文件。

#### src

- ExplorerEditor.cpp

  编辑器，继承Application，构建应用程序。

- EditorLayer.h/.cpp

  编辑器层，构建引擎编辑器。
  
- SceneHierarchyPanel.h/.cpp

  场景物体层级面板，显示所有场景物体及其层级关系。

### Sandbox 项目

#### asserts

- ##### shaders

  着色器文件，包括顶点着色器和片元着色器。

- ##### textures

  贴图文件。

#### src

- SandboxApp.cpp

  应用程序类，继承Application。可创建具体Layer。

- Sandbox2D.h/.cpp

  2D渲染测试层，继承Layer。

## 子系统

### 日志系统

### 事件系统

### 输入系统

### 渲染系统

