# pcm2wav

>**A tool convert audio pcm file to wav file**

**Build Instructions**

***Can use three method to build this project***
1. Use make tool to build this project in current directory
2. Use cmake tool
	- creat build directory in current directory
	- change directory to build
	- excute command `cmake ..`
3. Excute `compile.sh` in current dirctory  

**Usage**  
`pcm2wav pcm_file wav_file channels sample_rate sample_length`  

**Wav Format**
|偏移位置|字节数|类型|端序|说明|
|:-:|:-:|:-:|:-:|:-|
|0x00-0x03|4|字符|大端|"RIFF"块(0x52494646)，标记为RIFF文件格式|
|0x04-0x07|4|整数|小端|总数据量大小，从下一个地址开始，到文件末尾的总字节数|
|0x08-0x0B|4|字符|大端|类型码(Form Type)，WAV文件格式标记，即"WAVE"四个字母|
|0x0C-0x0F|4|字符|大端|"fmt "子块(0x666D7420)，注意末尾的空格|
|0x10-0x13|4|整数|小端|"fmt "子块数据域大小|
|0x14-0x15|2|整数|小端|编码格式，1代表PCM无损格式|
|0x16-0x17|2|整数|小端|声道数(Channels)，1或2|
|0x18-0x1B|4|整数|小端|采样率(Sample Rate)|
|0x1C-0x1F|4|整数|小端|每秒数据字节数(Byte Per Second)，等于采样率*每个采样所需的字节数|
|0x20-0x21|2|整数|小端|每个采样所需的字节数(Align)，等于采样位深*声道数/8|
|0x22-0x23|2|整数|小端|单个采样位深(Bits Per Sample)，可选8、16或32|
|0x24-0x27|2|整数|大端|"data"子块 (0x64617461)|
|0x28-0x2B|4|整数|小端|"data"子块数据域大小|
|0x2C-|N|||PCM|