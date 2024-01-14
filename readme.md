# CMSIS-DSP库的使用

## 源代码的获取

你要使用下面的命令从github中clone指定分支的源代码

```c
git clone -b v1.15.0 https://github.com/ARM-software/CMSIS-DSP.git
```

其中v1.15.0就是指定这个分支，你也可以选择其他分支

![image-20240114234712550](assets\image-20240114234712550.png)



必须的代码位于Include、PricvateInclude和Sources目录下

![image-20240114235159981](assets\image-20240114235159981.png)

## STM32CubeMX生成工程文件

我只初始化SPI1 Transmit Only Master模式和FreeRTOS其他什么都没做

即，在CubeMX中无需做关于DSP的任何操作，你只需要保证你的工程能够运行起来就行了

其中SPI用于驱动一个调试用的小屏幕

![image-20240114235808959](assets\image-20240114235808959.png)

可以跑起来后，再开始移植DSP Library

![1705249136413](assets\1705249136413.jpg)



## 开始移植DSP库

创建一个DSP目录，然后把Include、PrivateInclude、Sources这三个目录及其中的文件复制过来

![image-20240115003303854](assets\image-20240115003303854.png)

顺便提一下，这是移植以前，程序空间的占用情况

![image-20240115002359988](assets\image-20240115002359988.png)



接下来更改Cmake,使这三个文件中的代码参与编译

在include_directories()中添加.h文件的路径

![image-20240115003322730](assets\image-20240115003322730.png)

```c
DSP/Include
DSP/Include/dsp
DSP/PrivateInclude
```

由于在其每个单独功能的源代码下都有一个和其目录同名的.c文件，而这个.c文件中#include 了同级目录中的所有.c文件，因此我们需要在file中添加以下命令，避免编译重复

![image-20240115003344856](assets\image-20240115003344856.png)

```c
"DSP/Source/BasicMathFunctions/BasicMathFunctions.c"
"DSP/Source/BayesFunctions/BayesFunctions.c"
"DSP/Source/CommonTables/CommonTables.c"
"DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c"
"DSP/Source/ControllerFunctions/ControllerFunctions.c"
"DSP/Source/DistanceFunctions/DistanceFunctions.c"
"DSP/Source/FastMathFunctions/FastMathFunctions.c"
"DSP/Source/FilteringFunctions/FilteringFunctions.c"
"DSP/Source/InterpolationFunctions/InterpolationFunctions.c"
"DSP/Source/MatrixFunctions/MatrixFunctions.c.c"
"DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.c"
"DSP/Source/StatisticsFunctions/StatisticsFunctions.c"
"DSP/Source/SupportFunctions/SupportFunctions.c"
"DSP/Source/SVMFunctions/SVMFunctions.c"
"DSP/Source/TransformFunctions/TransformFunctions.c"
"DSP/Source/WindowFunctions/WindowFunctions.c"
```



然后，更新下cmake,就可以进行编译了

![image-20240115003609420](assets\image-20240115003609420.png)

![image-20240115003510036](assets\image-20240115003510036.png)

可以看到，确实参与了编译。

但我们还没有开始使用，可能是编译器优化了，这次编译的程序和还没有移植前的大小一样

由此，移植就算结束了

## DSP库的使用

现在我们使用DSP中的fft库进行一个快速傅里叶变化

在app_freertos.c文件中添加以下代码

```c
/* USER CODE BEGIN Includes */
#include "lcd.h"	//LCD显示
#include "arm_math.h"	//DSP相关头文件
#include "arm_const_structs.h"	//DSP中的常量
/* USER CODE END Includes */
```

```c
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
#define FFT_LENGTH  1024	
//定义两个数组,注意这个数组的存储期是静态存储期。定义一个全局变量就行
float fft_inputbuf[FFT_LENGTH*2];
float fft_outputbuf[FFT_LENGTH];
/* USER CODE END Variables */
```

```c
/* USER CODE BEGIN Header_App_main */
/**
  * @brief  Function implementing the main thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_App_main */
void App_main(void *argument) {
    /* USER CODE BEGIN App_main */
    LCD_ShowString(30,0,"DSP test",BLACK,WHITE,24,0);

    int i = 0;
    //gennerat signal
    for (i = 0; i < FFT_LENGTH; i++) {
        fft_inputbuf[2 * i] = 100 + 10 * arm_sin_f32(2 * PI * i * 10 / FFT_LENGTH) + 20 * arm_sin_f32(2 * PI * i * 50 / FFT_LENGTH) + 30 * arm_cos_f32(2 * PI * i * 300 / FFT_LENGTH);
        fft_inputbuf[2 * i + 1] = 0;
    }

    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputbuf, 0, 1);
    arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);

    LCD_ShowIntNum(30, 24, (int)fft_outputbuf[0], 8, BLACK, WHITE, 24);
    LCD_ShowIntNum(30, 48, (int)fft_outputbuf[10], 8, BLACK, WHITE, 24);
    /* Infinite loop */
    for (;;) {

        vTaskDelay(100);
    }
    /* USER CODE END App_main */
}
```

编译以后得到的程序占用情况

![image-20240115005951902](assets\image-20240115005951902.png)

你可以看到，占用立刻就起来了

顺便说一下，那两个全局数据占用12288Byte的Flash

如果你想减少占用可以在cmake中将不用的功能函数的目录先注释掉，但实测下来没用

如图，我已经将能注释掉的目录注释掉了（没注释掉的为必须需要的代码）

![image-20240115011628648](assets\image-20240115011628648.png)

编译的结果：

![image-20240115011732281](assets\image-20240115011732281.png)

可以看到程序的占用并未改变，由此说明编译器已经进行了优化，未使用的功能虽然参与编译但不会参与构建

因此你就直接包括所有目录吧，其他的交给编译器

### 然后我们来验证一下结果是否正确

![1705256190684](assets\1705256190684.jpg)

下面是Debug模式下，编译器显示的结果

![image-20240115021913180](assets\image-20240115021913180.png)

![image-20240115021927291](assets\image-20240115021927291.png)

结果没问题

## 这段代码的运行时间

修改一下main函数中的代码，以便使用示波器测量代码运行时间

![image-20240115032959071](assets\image-20240115032959071.png)

![新建文件1](assets\新建文件1.png)

大致得出这两行代码的运行时间是1.1ms