encodingshow
============

show raw data in memory

1. 正常模式
	* 输入：文本内容。"你好啊"
	* 输出：对应文本内容，在各种编码情况下的存储内容，以unsigned short为单位显示，小头序。
2. 非正常模式
	* 输入：编码####编码内容。 "utf16####0xFEFF 0x5E05 0x54E5"
	* 输出：对应文本内容，在各种编码情况下的存储内容，以unsigned short为单位显示，小头序。
3. 判断真实文本内容
	* 输入：checkraw##看过去的文本内容。"checkraw##涔犺繎骞崇殑濂冲効"
	* 输出：把文本当做gbk时候的样子，把文本当做utf8时候的样子。
