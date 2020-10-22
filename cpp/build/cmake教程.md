
<!-- vim-markdown-toc GFM -->

* [CMake教程 25](#cmake教程-25)
	* [基本起点（步骤1） 31](#基本起点步骤1-31)
		* [添加一个版本号和配置的头文件 49](#添加一个版本号和配置的头文件-49)
		* [指定C ++标准 98](#指定c-标准-98)
		* [构建和测试 122](#构建和测试-122)
	* [添加一个库（步骤2） 139](#添加一个库步骤2-139)
	* [添加库的使用要求（步骤3） 242](#添加库的使用要求步骤3-242)
	* [安装和测试（步骤4） 283](#安装和测试步骤4-283)
		* [安装规则 287](#安装规则-287)
		* [测试支持 315](#测试支持-315)
	* [Adding System Introspection (Step 5) 366](#adding-system-introspection-step-5-366)
		* [指定编译定义 440](#指定编译定义-440)
	* [添加自定义命令和生成的文件（步骤6） 476](#添加自定义命令和生成的文件步骤6-476)
	* [构建安装程序（步骤7） 556](#构建安装程序步骤7-556)
	* [添加对仪表盘的支持（步骤8） 593](#添加对仪表盘的支持步骤8-593)
	* [混合静态和共享（步骤9） 642](#混合静态和共享步骤9-642)
	* [添加生成器表达式（步骤10） 823](#添加生成器表达式步骤10-823)
	* [添加导出配置（步骤11） 877](#添加导出配置步骤11-877)
	* [打包调试和发布（步骤12） 971](#打包调试和发布步骤12-971)

<!-- vim-markdown-toc -->
# CMake教程 25
	CMake教程提供了分步指南，涵盖了CMake可以解决的常见构建系统问题。
了解示例项目中各个主题如何协同工作将非常有帮助。教程文档和示例的源代码可
以Help/guide/tutorial在CMake源代码树的目录中找到。每个步骤都有其自己的子目录，
该目录包含可以用作起点的代码。教程示例是渐进式的，因此每个步骤都为上一步提供了完整的解决方案

## 基本起点（步骤1） 31
1. 最基本的项目是从源代码文件构建的可执行文件。对于简单的项目，
只需三行CMakeLists.txt文件。这将是本教程的起点。
CMakeLists.txt在Step1目录中创建一个文件，如下所示：

```
cmake_minimum_required(VERSION 3.10)

// set the project name
project(Tutorial)

// add the executable
add_executable(Tutorial tutorial.cxx)
```

2. 请注意，此示例在CMakeLists.txt文件中使用小写命令。CMake支持大写，小写和大小写混合命令。目录的源
代码tutorial.cxx已提供，Step1可用于计算数字的平方根。

### 添加一个版本号和配置的头文件 49

我们将添加的第一个功能是为我们的可执行文件和项目提供版本号。虽然我们可以在源代码中专门执行此操
作，但是使用 CMakeLists.txt可以提供更大的灵活性。

首先，修改CMakeLists.txt文件以使用project() 命令设置项目名称和版本号。
```
cmake_minimum_required(VERSION 3.10)

# set the project name and version 58
project(Tutorial VERSION 1.0)
```

然后，配置头文件以将版本号传递给源代码：

```
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

由于已配置的文件将被写入二进制树，因此我们必须将该目录添加到路径列表中以搜索包含文件。将以下行
添加到CMakeLists.txt文件的末尾：

```
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

使用您喜欢的编辑器，TutorialConfig.h.in在源目录中创建以下内容：
```
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@ 80
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@ 81

当CMake配置此头文件时，@Tutorial_VERSION_MAJOR@和的值 @Tutorial_VERSION_MINOR@将被替换。

接下来修改tutorial.cxx以包括配置的头文件 TutorialConfig.h。

最后，让我们通过tutorial.cxx如下更新来打印出版本号：

  if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }
```

### 指定C ++标准 98

接下来，我们通过更换添加一些C ++ 11功能，我们的项目atof与 std::stod在tutorial.cxx。同时，删除。
```
#include <cstdlib> 102

	const double inputValue = std::stod(argv[1]);
```

我们将需要在CMake代码中明确声明应使用正确的标志。在CMake中启用对特定C ++标准的支持的最简单方法
是使用CMAKE_CXX_STANDARD变量。对于本教程，请设置CMAKE_CXX_STANDARD将CMakeLists.txt文件中的变量
设置为11并CMAKE_CXX_STANDARD_REQUIRED 改为True：

```
cmake_minimum_required(VERSION 3.10)

# set the project name and version 114
project(Tutorial VERSION 1.0)

# specify the C++ standard 117
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

### 构建和测试 122

跑过 cmake 可执行文件或 cmake-gui 配置项目，然后使用所选的构建工具进行构建。

例如，从命令行我们可以导航到Help/guide/tutorialCMake源代码树的目录并运行以下命令：

mkdir Step1_build
cd Step1_build
cmake ../Step1
cmake --build .

导航到构建Tutorial的目录（可能是make目录或Debug或Release构建配置子目录），然后运行以下命令：

Tutorial 4294967296
Tutorial 10
Tutorial

## 添加一个库（步骤2） 139

现在，我们将向我们的项目添加一个库。该库将包含我们自己的实现，用于计算数字的平方根。然后可执行
文件可以使用此库而不是编译器提供的标准平方根函数。

在本教程中，我们将库放入名为的子目录中MathFunctions。该目录已经包含一个头文件 MathFunctions.h和
一个源文件mysqrt.cxx。源文件具有一个称为的mysqrt功能，该功能提供与编译器sqrt功能相似的功能。

将以下一个行CMakeLists.txt文件添加到MathFunctions 目录：

add_library(MathFunctions mysqrt.cxx)

为了利用新库，我们将添加一个 add_subdirectory() 调用顶级CMakeLists.txt文件，以便构建库。我们将
新库添加到可执行文件，并添加MathFunctions为包含目录，以便mqsqrt.h可以找到头文件。现在，顶级
CMakeLists.txt文件的最后几行应如下所示：

```
# add the MathFunctions library 156
add_subdirectory(MathFunctions)

# add the executable 159
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions)

# add the binary tree to the search path for include files 164
# so that we will find TutorialConfig.h 165
target_include_directories(Tutorial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/MathFunctions"
                          )
```

现在让我们将MathFunctions库设为可选。虽然对于本教程而言确实没有任何必要，但是对于较大的项目，这
是常见的情况。第一步是向顶层CMakeLists.txt文件添加一个选项。

```
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings 178
# to the source code 179
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

此选项将显示在 cmake-gui 和 ccmake 用户可以更改的默认值ON。此设置将存储在缓存中，因此用户无需在
每次在构建目录上运行CMake时都设置该值。

下一个更改是使建立和链接MathFunctions库成为条件。为此，我们将顶级CMakeLists.txt 文件的结尾更改
为如下所示：

```
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# add the executable 196
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files 201
# so that we will find TutorialConfig.h 202
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
```

请注意，使用变量EXTRA_LIBS来收集所有可选库，以便以后链接到可执行文件中。该变量 EXTRA_INCLUDES类
似地用于可选的头文件。当处理许多可选组件时，这是一种经典方法，我们将在下一步中介绍现代方法。

对源代码的相应更改非常简单。首先，如果需要，请在tutorial.cxx中包含MathFunctions.h标题：

```
#ifdef USE_MYMATH 215
#  include "MathFunctions.h" 216
#endif 217

然后，在同一文件中，USE_MYMATH控制使用哪个平方根函数：

#ifdef USE_MYMATH 221
  const double outputValue = mysqrt(inputValue);
#else 223
  const double outputValue = sqrt(inputValue);
#endif 225
```

由于源代码现在需要，USE_MYMATH我们可以TutorialConfig.h.in使用以下行将其添加到：

```
#cmakedefine USE_MYMATH 231
```

练习：为什么TutorialConfig.h.in 在选项之后进行配置很重要USE_MYMATH？如果我们将两者倒置会怎样？

跑过 cmake 可执行文件或 cmake-gui配置项目，然后使用所选的构建工具进行构建。然后运行构建的
Tutorial可执行文件。

使用 ccmake 可执行文件或 cmake-gui 更新的值USE_MYMATH。重新生成并再次运行本教程。sqrt或mysqrt哪
个函数可提供更好的结果？

## 添加库的使用要求（步骤3） 242

使用要求可以更好地控制库或可执行文件的链接并包含行，同时还可以更好地控制CMake内部目标的传递属性
。利用使用需求的主要命令是：

      □ target_compile_definitions()

      □ target_compile_options()

      □ target_include_directories()

      □ target_link_libraries()

让我们从添加库（第2步）中重构代码，以使用现代的CMake使用需求方法。我们首先声明，链接到
MathFunctions的任何人都需要包括当前源目录，而MathFunctions本身不需要。因此这可能成为INTERFACE使
用要求。

记住INTERFACE是指消费者需要的东西，而生产者则不需要。将以下行添加到的末尾 MathFunctions/
CMakeLists.txt：

target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )

现在，我们已经指定了MathFunction的使用要求，我们可以安全地EXTRA_INCLUDES从顶级 CMakeLists.txt（
这里为：

if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
endif()

和这里：

target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

完成此操作后，运行 cmake 可执行文件或 cmake-gui配置项目，然后使用您选择的构建工具或通过构建目录
进行构建。cmake --build .

## 安装和测试（步骤4） 283

现在，我们可以开始向我们的项目添加安装规则和测试支持。

### 安装规则 287

安装规则非常简单：对于MathFunctions，我们要安装库和头文件，对于应用程序，我们要安装可执行文件和
配置的头文件。

因此，MathFunctions/CMakeLists.txt我们最后添加：

install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)

并在顶层末尾CMakeLists.txt添加：

install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include
  )

这就是创建本教程的基本本地安装所需的全部。

跑过 cmake 可执行文件或 cmake-gui配置项目，然后使用所选的构建工具进行构建。使用以下install 选项
运行安装步骤cmake命令（在3.15中引入，较早版本的CMake必须使用），或者从IDE 构建目标。这将安装适
当的头文件，库和可执行文件。make installINSTALL

CMake变量 CMAKE_INSTALL_PREFIX用于确定文件的安装根目录。如果使用定制安装目录，则可以通过参数指
定。对于多配置工具，请使用参数指定配置。cmake --install--prefix--config

验证已安装的教程正在运行。

### 测试支持 315

接下来让我们测试我们的应用程序。在顶级CMakeLists.txt 文件的末尾，我们可以启用测试，然后添加一些
基本测试以验证应用程序是否正常运行。

```
enable_testing()

# does the application run 323
add_test(NAME Runs COMMAND Tutorial 25)

# does the usage message work? 326
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
  )

# define a function to simplify adding tests 332
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

# do a bunch of result based tests 340
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is [-nan|nan|0]")
do_test(Tutorial 0.0001 "0.0001 is 0.01")
```

第一个测试只是简单地验证应用程序正在运行，没有段错误或其他崩溃，并且返回值为零。这是CTest测试的
基本形式。

下一个测试利用 PASS_REGULAR_EXPRESSION测试属性，以验证测试的输出包含某些字符串。在这种情况下，
请验证在提供了错误数量的参数时是否打印了用法消息。

Lastly, we have a function called do_test that runs the application and verifies that the
computed square root is correct for given input. For each invocation of do_test, another test
is added to the project with a name, input, and expected results based on the passed arguments.

Rebuild the application and then cd to the binary directory and run the ctest executable: ctest
-N and ctest -VV. For multi-config generators (e.g. Visual Studio), the configuration type must
be specified. To run tests in Debug mode, for example, use ctest -C Debug -VV from the build
directory (not the Debug subdirectory!). Alternatively, build the RUN_TESTS target from the
IDE.

## Adding System Introspection (Step 5) 366

Let us consider adding some code to our project that depends on features the target platform
may not have. For this example, we will add some code that depends on whether or not the target
platform has the log and exp functions. Of course almost every platform has these functions but
for this tutorial assume that they are not common.

If the platform has log and exp then we will use them to compute the square root in the mysqrt
function. We first test for the availability of these functions using the CheckSymbolExists
module in the top-level CMakeLists.txt. On some platforms, we will need to link to the m
library. If log and exp are not initially found, require the m library and try again.

我们将在中使用新的定义TutorialConfig.h.in，因此请确保在配置该文件之前进行设置。

include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if(NOT (HAVE_LOG AND HAVE_EXP))
  unset(HAVE_LOG CACHE)
  unset(HAVE_EXP CACHE)
  set(CMAKE_REQUIRED_LIBRARIES "m")
  check_symbol_exists(log "math.h" HAVE_LOG)
  check_symbol_exists(exp "math.h" HAVE_EXP)
  if(HAVE_LOG AND HAVE_EXP)
    target_link_libraries(MathFunctions PRIVATE m)
  endif()
endif()

现在，将这些定义添加到，TutorialConfig.h.in以便我们可以从中使用它们mysqrt.cxx：

```
// does the platform provide exp and log functions?
#cmakedefine HAVE_LOG 398
#cmakedefine HAVE_EXP 399
```

如果log和exp在系统上可用，那么我们将使用它们来计算函数中的平方根mysqrt。将以下代码添加到中的
mysqrt函数中MathFunctions/mysqrt.cxx（#endif返回结果前不要忘了！）：

```
#if defined(HAVE_LOG) && defined(HAVE_EXP) 406
  double result = exp(log(x) * 0.5);
  std::cout << "Computing sqrt of " << x << " to be " << result
            << " using log and exp" << std::endl;
#else 410
  double result = x;
```

我们还需要修改mysqrt.cxx以包含cmath。

```
#include <cmath> 417
```

跑过 cmake 可执行文件或 cmake-gui 配置项目，然后使用所选的构建工具进行构建，并运行Tutorial可执
行文件。

您会注意到我们没有使用log和exp，即使我们认为它们应该可用。我们应该很快认识到，我们都忘记了，包
括TutorialConfig.h在mysqrt.cxx。

我们还需要更新，MathFunctions/CMakeLists.txt以便mysqrt.cxx 知道此文件的位置：

```
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE ${CMAKE_BINARY_DIR}
          )
```

完成此更新后，继续并再次构建项目，然后运行构建的Tutorial可执行文件。如果log和exp仍未使用，请
TutorialConfig.h从构建目录打开生成的文件。也许它们在当前系统上不可用？

sqrt或mysqrt哪个函数现在可以提供更好的结果？

### 指定编译定义 440

除了in之外，还有更好的地方供我们保存HAVE_LOG和HAVE_EXP值TutorialConfig.h吗？让我们尝试使用 
target_compile_definitions()。

首先，从中删除定义TutorialConfig.h.in。我们不再需要包含TutorialConfig.hfrom mysqrt.cxx或多余的
include in MathFunctions/CMakeLists.txt。

接下来，我们可以将check HAVE_LOG和HAVE_EXP移至 MathFunctions/CMakeLists.txt，然后将这些值指定为
PRIVATE 编译定义。

```
include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if(NOT (HAVE_LOG AND HAVE_EXP))
  unset(HAVE_LOG CACHE)
  unset(HAVE_EXP CACHE)
  set(CMAKE_REQUIRED_LIBRARIES "m")
  check_symbol_exists(log "math.h" HAVE_LOG)
  check_symbol_exists(exp "math.h" HAVE_EXP)
  if(HAVE_LOG AND HAVE_EXP)
    target_link_libraries(MathFunctions PRIVATE m)
  endif()
endif()

# add compile definitions 466
if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()
```

完成这些更新后，继续并重新构建项目。运行内置的Tutorial可执行文件，并验证结果与本步骤前面的内容
相同。

## 添加自定义命令和生成的文件（步骤6） 476

假设，出于本教程的目的，我们决定不再使用平台log和exp函数，而是希望生成一个可在mysqrt函数中使用
的预计算值表。在本节中，我们将在构建过程中创建表，然后将该表编译到我们的应用程序中。

首先，让我们删除中对log和exp功能的检查MathFunctions/CMakeLists.txt。然后取出支票HAVE_LOG和 
HAVE_EXP从mysqrt.cxx。同时，我们可以删除。#include <cmath>

在MathFunctions子目录中，提供了一个名为的新源文件 MakeTable.cxx来生成表。

查看完文件后，我们可以看到该表是作为有效的C ++代码生成的，并且输出文件名作为参数传入。

下一步是将适当的命令添加到 MathFunctions/CMakeLists.txt文件中，以构建MakeTable可执行文件，然后
在构建过程中运行它。需要一些命令来完成此操作。

首先，在的顶部MathFunctions/CMakeLists.txt，MakeTable添加的可执行文件，就像添加任何其他可执行文
件一样。

add_executable(MakeTable MakeTable.cxx)

然后，我们添加一个自定义命令，该命令指定如何Table.h 通过运行MakeTable 进行生产。

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )

接下来，我们必须让CMake知道这mysqrt.cxx取决于生成的文件Table.h。这是通过将生成的添加Table.h到库
MathFunctions的源列表中来完成的。

add_library(MathFunctions
            mysqrt.cxx
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            )

我们还必须将当前的二进制目录添加到包含目录的列表中，以便Table.h可以找到和包含它mysqrt.cxx。

target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
          )


现在，让我们使用生成的表。首先，修改mysqrt.cxx为 Table.h。接下来，我们可以重写mysqrt函数以使用
该表：

double mysqrt(double x)
{
  if (x <= 0) {
    return 0;
  }

  // use the table to help find an initial value
  double result = x;
  if (x >= 1 && x < 10) {
    std::cout << "Use the table to help find an initial value " << std::endl;
    result = sqrtTable[static_cast<int>(x)];
  }

  // do ten iterations
  for (int i = 0; i < 10; ++i) {
    if (result <= 0) {
      result = 0.1;
    }
    double delta = x - (result * result);
    result = result + 0.5 * delta / result;
    std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
  }

  return result;
}

跑过 cmake 可执行文件或 cmake-gui 配置项目，然后使用所选的构建工具进行构建。

构建此项目时，它将首先构建MakeTable可执行文件。然后它将运行MakeTable产生Table.h。最后，它将进行
编译mysqrt.cxx，其中包括Table.h生成MathFunctions库的过程。

运行Tutorial可执行文件，并验证它是否正在使用该表。

## 构建安装程序（步骤7） 556

接下来，假设我们想将项目分发给其他人，以便他们可以使用它。我们希望在各种平台上提供二进制和源代
码分发。这与之前在“ 安装和测试”（第4步）中进行的安装有些不同，在安装和测试中，我们正在安装根据
源代码构建的二进制文件。在此示例中，我们将构建支持二进制安装和程序包管理功能的安装程序包。为此
，我们将使用CPack创建平台特定的安装程序。具体来说，我们需要在顶级CMakeLists.txt文件的底部添加几
行。

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include(CPack)

这就是全部。我们首先包括 InstallRequiredSystemLibraries。该模块将包含项目当前平台所需的任何运行
时库。接下来，我们将一些CPack变量设置为存储该项目的许可证和版本信息的位置。版本信息是在本教程的
前面设置的，并且license.txt已包含在此步骤的顶级源目录中。

最后，我们将 CPack module 它将使用这些变量和当前系统的其他一些属性来设置安装程序。

下一步是按通常的方式构建项目，然后运行 cpack可执行文件。要构建二进制发行版，请从二进制目录运行
：

cpack

要指定生成器，请使用-G选项。对于多配置版本，用于 -C指定配置。例如：

cpack -G ZIP -C Debug

要创建源分发，您可以输入：

cpack --config CPackSourceConfig.cmake

或者，从IDE 运行或右键单击目标。make packagePackageBuild Project

运行在二进制目录中找到的安装程序。然后运行已安装的可执行文件，并验证其是否有效。

## 添加对仪表盘的支持（步骤8） 593

添加将测试结果提交到仪表板的支持非常简单。我们已经在“ 测试支持”中为我们的项目定义了许多测试。现
在，我们只需要运行这些测试并将其提交到仪表板即可。为了包括对仪表板的支持，我们将CTest顶层模块 
CMakeLists.txt。

更换：

```
# enable testing 602
enable_testing()
```

带有：

```
# enable dashboard scripting 609
include(CTest)
```

的 CTest模块将自动调用enable_testing()，因此我们可以将其从CMake文件中删除。

我们还需要CTestConfig.cmake在顶级目录中创建一个文件，在其中可以指定项目的名称以及提交仪表板的位
置。

```
set(CTEST_PROJECT_NAME "CMakeTutorial")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=CMakeTutorial")
set(CTEST_DROP_SITE_CDASH TRUE)
```

的 ctest可执行文件将在运行时读入此文件。要创建一个简单的仪表板，您可以运行cmake 可执行文件或 
cmake-gui配置项目，但尚未构建。而是，将目录更改为二进制树，然后运行：

    ctest [-VV] -D实验

请记住，对于多配置生成器（例如Visual Studio），必须指定配置类型：

ctest [-VV] -C Debug -D Experimental

或者，从IDE中构建Experimental目标。

的 ctest可执行文件将构建并测试项目，并将结果提交到Kitware的公共仪表板：https ://my.cdash.org/
index.php?project = CMakeTutorial 。

## 混合静态和共享（步骤9） 642

在本节中，我们将展示 BUILD_SHARED_LIBS 变量可用于控制默认行为 add_library()，并允许在如何没有显
式类型库（控制STATIC， SHARED，MODULE或OBJECT）构建的。

为此，我们需要添加 BUILD_SHARED_LIBS到顶层CMakeLists.txt。我们使用option() 命令，因为它允许用户
选择该值是ON还是OFF。

接下来，我们将重构MathFunctions使其成为使用mysqrt或封装的真实库sqrt，而不是要求调用代码执行此逻
辑。这也将意味着USE_MYMATH它将不控制构建MathFunction，而是将控制此库的行为。

第一步是将顶层的开始部分更新为 CMakeLists.txt：

```
cmake_minimum_required(VERSION 3.10)

# set the project name and version 658
project(Tutorial VERSION 1.0)

# specify the C++ standard 661
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows 665
# we don't need to tinker with the path to run the executable 666
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# configure a header file to pass the version number only 673
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library 676
add_subdirectory(MathFunctions)

# add the executable 679
add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC MathFunctions)
```

既然我们已经使MathFunctions始终被使用，我们将需要更新该库的逻辑。因此，MathFunctions/
CMakeLists.txt我们需要创建一个SqrtLibrary，USE_MYMATH启用后将有条件地对其进行构建。现在，由于这
是一个教程，我们将明确要求静态地构建SqrtLibrary。

最终结果MathFunctions/CMakeLists.txt应为：

```
# add the library that runs 691
add_library(MathFunctions MathFunctions.cxx)

# state that anybody linking to us needs to include the current source dir 694
# to find MathFunctions.h, while we don't. 695
target_include_directories(MathFunctions
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

# should we use our own math functions 700
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if(USE_MYMATH)

  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  # first we add the executable that generates the table
  add_executable(MakeTable MakeTable.cxx)

  # add the command to generate the source code
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    DEPENDS MakeTable
    )

  # library that just does sqrt
  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              ${CMAKE_CURRENT_BINARY_DIR}/Table.h
              )

  # state that we depend on our binary dir to find Table.h
  target_include_directories(SqrtLibrary PRIVATE
                             ${CMAKE_CURRENT_BINARY_DIR}
                             )

  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

# define the symbol stating we are using the declspec(dllexport) when 730
# building on windows 731
target_compile_definitions(MathFunctions PRIVATE "EXPORTING_MYMATH")

# install rules 734
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

接下来，更新MathFunctions/mysqrt.cxx为使用mathfunctions和 detail名称空间：

```
#include <iostream> 742

#include "MathFunctions.h" 744

// include the generated table
#include "Table.h" 747

namespace mathfunctions {
namespace detail {
// a hack square root calculation using simple operations
double mysqrt(double x)
{
  if (x <= 0) {
    return 0;
  }

  // use the table to help find an initial value
  double result = x;
  if (x >= 1 && x < 10) {
    std::cout << "Use the table to help find an initial value " << std::endl;
    result = sqrtTable[static_cast<int>(x)];
  }

  // do ten iterations
  for (int i = 0; i < 10; ++i) {
    if (result <= 0) {
      result = 0.1;
    }
    double delta = x - (result * result);
    result = result + 0.5 * delta / result;
    std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
  }

  return result;
}
}
}
```

我们还需要对中进行一些更改tutorial.cxx，以使其不再使用USE_MYMATH：

 1. 一律包括 MathFunctions.h

 2. 一律使用 mathfunctions::sqrt

 3. 不包括cmath

最后，更新MathFunctions/MathFunctions.h为使用dll导出定义：

```
#if defined(_WIN32) 792
#  if defined(EXPORTING_MYMATH) 793
#    define DECLSPEC __declspec(dllexport) 794
#  else 795
#    define DECLSPEC __declspec(dllimport) 796
#  endif 797
#else // non windows 798
#  define DECLSPEC 799
#endif 800

namespace mathfunctions {
double DECLSPEC sqrt(double x);
}
```

此时，如果您构建了所有内容，则会注意到链接失败，因为我们将没有位置独立代码的静态库与具有位置独
立代码的库组合在一起。解决方案是显式设置POSITION_INDEPENDENT_CODE 无论构建类型如何，SqrtLibrary
的target属性都为True。

```
  # state that SqrtLibrary need PIC when the default is shared libraries
  set_target_properties(SqrtLibrary PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                        )

  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
```

练习：我们修改MathFunctions.h为使用dll导出定义。使用CMake文档，您可以找到一个帮助器模块来简化此
过程吗？

## 添加生成器表达式（步骤10） 823

Generator expressions 在生成系统的过程中进行评估，以生成特定于每个生成配置的信息。

Generator expressions 在许多目标属性的上下文中是允许的，例如 LINK_LIBRARIES， 
INCLUDE_DIRECTORIES， COMPILE_DEFINITIONS和别的。在使用命令填充这些属性时，也可以使用它们，例如
target_link_libraries()， target_include_directories()， target_compile_definitions() 和别的。

Generator expressions 可以用于启用条件链接，编译时使用的条件定义，条件包含目录等。条件可以基于
构建配置，目标属性，平台信息或任何其他可查询信息。

有不同类型的 generator expressions 包括逻辑，信息和输出表达式。

逻辑表达式用于创建条件输出。基本表达式是0和1表达式。一个$<0:...>导致空字符串，而<1:...>导致的内
容“...”。它们也可以嵌套。

的常见用法 generator expressions是有条件地添加编译器标志，例如用于语言级别或警告的标志。一个不
错的模式是将该信息与INTERFACE 目标关联，以允许该信息传播。让我们从构造一个 INTERFACE目标并指定
所需的C ++标准级别开始，11 而不是使用CMAKE_CXX_STANDARD。

所以下面的代码：

```
# specify the C++ standard 846
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

将被替换为：

```
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)
```

接下来，我们为项目添加所需的编译器警告标志。由于警告标志根据编译器的不同而不同，因此我们使用
COMPILE_LANG_AND_ID 生成器表达式来控制在给定一种语言和一组编译器ID的情况下应用哪些标志，如下所
示：

```
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
```

对此，我们看到警告标志封装在 BUILD_INTERFACE条件中。这样做是为了使我们已安装项目的使用者不会继
承我们的警告标志。

练习：修改MathFunctions/CMakeLists.txt以使所有目标都有一个target_link_libraries()致电至
tutorial_compiler_flags。

## 添加导出配置（步骤11） 877

在教程的“ 安装和测试”（第4步）中，我们添加了CMake的功能，以安装项目的库和头文件。在构建安装程序
（第7步）期间，我们添加了打包此信息的功能，以便可以将其分发给其他人。

下一步是添加必要的信息，以便其他CMake项目可以使用我们的项目，无论是从构建目录，本地安装还是打包
时。

第一步是更新我们的 install(TARGETS)命令不仅指定一个，DESTINATION而且还指定一个EXPORT。该EXPORT
关键字生成并安装包含代码导入从安装树安装命令列出的所有目标的CMake的文件。因此，让我们继续进行操
作，并EXPORT通过更新以下install 命令MathFunctions/CMakeLists.txt来显式地显示MathFunctions库：

install(TARGETS MathFunctions tutorial_compiler_flags
        DESTINATION lib
        EXPORT MathFunctionsTargets)
install(FILES MathFunctions.h DESTINATION include)

现在我们已经导出了MathFunctions，我们还需要显式安装生成的MathFunctionsTargets.cmake文件。这是通
过在顶层底部添加以下内容来完成的CMakeLists.txt：

install(EXPORT MathFunctionsTargets
  FILE MathFunctionsTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)

此时，您应该尝试运行CMake。如果一切设置正确，您将看到CMake将生成如下错误：

Target "MathFunctions" INTERFACE_INCLUDE_DIRECTORIES property contains
path:

  "/Users/robert/Documents/CMakeClass/Tutorial/Step11/MathFunctions"

which is prefixed in the source directory.

CMake试图说的是，在生成导出信息的过程中，它将导出与当前机器固有联系的路径，并且在其他机器上无效
。解决方案是更新MathFunctionstarget_include_directories()了解INTERFACE在构建目录和安装/软件包中
使用它时需要在不同的位置。这意味着转换 target_include_directories() 要求MathFunctions看起来像：

target_include_directories(MathFunctions
                           INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:include>
                           )

更新后，我们可以重新运行CMake并确认它不再发出警告。

在这一点上，我们已经正确地包装了CMake所需的目标信息，但是我们仍然需要生成一个
MathFunctionsConfig.cmake使CMakefind_package()命令可以找到我们的项目。因此，我们继续将新文件添
加到项目的顶层，该文件 Config.cmake.in具有以下内容：

@PACKAGE_INIT@

include ( "${CMAKE_CURRENT_LIST_DIR}/MathFunctionsTargets.cmake" )

然后，要正确配置和安装该文件，请将以下内容添加到顶层的底部CMakeLists.txt：

```
install(EXPORT MathFunctionsTargets
  FILE MathFunctionsTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports 940
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )
# generate the version file for the config file 947
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfigVersion.cmake"
  VERSION "${Tutorial_VERSION_MAJOR}.${Tutorial_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file 954
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake
  DESTINATION lib/cmake/MathFunctions
  )
```

至此，我们为项目生成了可重定位的CMake配置，可以在安装或打包项目后使用它。如果我们也希望从构建目
录中使用我们的项目，则只需将以下内容添加到顶层的底部CMakeLists.txt：

export(EXPORT MathFunctionsTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsTargets.cmake"
)

通过此导出调用，我们现在生成一个Targets.cmake，允许MathFunctionsConfig.cmake构建目录中的配置供
其他项目使用，而无需安装它。

## 打包调试和发布（步骤12） 971

注意：此示例对单配置生成器有效，而对多配置生成器（例如Visual Studio）无效。

默认情况下，CMake的模型是构建目录仅包含一个配置，可以是Debug，Release，MinSizeRel或
RelWithDebInfo。但是，可以将CPack设置为捆绑多个构建目录，并构建一个包含同一项目的多个配置的软件
包。

首先，我们要确保调试版本和发行版本对要安装的可执行文件和库使用不同的名称。让我们将d用作调试可执
行文件和库的后缀。

组 CMAKE_DEBUG_POSTFIX在顶级CMakeLists.txt文件开头附近：

set(CMAKE_DEBUG_POSTFIX d)

add_library(tutorial_compiler_flags INTERFACE)

和 DEBUG_POSTFIX 教程可执行文件上的属性：

add_executable(Tutorial tutorial.cxx)
set_target_properties(Tutorial PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(Tutorial PUBLIC MathFunctions)

让我们还将版本编号添加到MathFunctions库中。在中 MathFunctions/CMakeLists.txt，设置VERSION 和 
SOVERSION 特性：

set_property(TARGET MathFunctions PROPERTY VERSION "1.0.0")
set_property(TARGET MathFunctions PROPERTY SOVERSION "1")

在Step12目录中，创建debug和release 子目录。布局将如下所示：

- Step12
   - debug
   - release

现在我们需要设置调试和发布版本。我们可以用 CMAKE_BUILD_TYPE 设置配置类型：

cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
cd ../release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

既然调试和发布版本均已完成，我们就可以使用自定义配置文件将两个版本打包到一个发布中。在 Step12目
录中，创建一个名为的文件MultiCPackConfig.cmake。在此文件中，首先包括由配置文件创建的默认配置文
件。 cmake 可执行文件。

接下来，使用CPACK_INSTALL_CMAKE_PROJECTS变量指定要安装的项目。在这种情况下，我们要同时安装调试
和发布。

include("release/CPackConfig.cmake")

set(CPACK_INSTALL_CMAKE_PROJECTS
    "debug;Tutorial;ALL;/"
    "release;Tutorial;ALL;/"
    )

从Step12目录运行cpack使用以下config选项指定我们的自定义配置文件：

cpack --config MultiCPackConfig.cmake

