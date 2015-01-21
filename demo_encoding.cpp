/*
 * =====================================================================================
 *
 *       Filename:  character_iconv.cpp
 *
 *    Description:  have two parameter,first is from set ,second is to set.
 *
 *        Version:  1.0
 *        Created:  10/22/2012 05:51:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wangzhen (newle), wangzhensi0487@sogou-inc.com
 *        Company:  sogou-inc
 *
 */

#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <assert.h>

static bool isDigit(const char c) {
	if (c >= '0' && c <= '9') {
		return true;
	} else if (c >= 'a' && c <= 'f') {
		return true;
	} else if (c >= 'A' && c <= 'F') {
		return true;
	}
	return false;
}

static unsigned char char2Val(const char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	return 0;
}
static char val2Char(const unsigned char c)
{
	if(c >= 0 && c <= 9) {
		return '0'+c;
	} else if(c >= 10) {
		return 'A' + c - 10;
	}
}

size_t disDump(const std::string resultContent, std::string& rawResultContent, const char* separate, const size_t len)
{
	size_t pos = 0;
	for(size_t i = 0; i < resultContent.length(); i += 2) {
		for(size_t j = 0; j < len; j++) {
			rawResultContent[pos++] = separate[j];
		}
		//是由于小头存储，对于unsigned short，策略是实际低位放在内存中的低位
		rawResultContent[pos++] = val2Char(((unsigned char)resultContent[i+1]) >> 4);
		rawResultContent[pos++] = val2Char(((unsigned char)resultContent[i+1]) & 0x0F);	
		rawResultContent[pos++] = val2Char(((unsigned char)resultContent[i]) >> 4);
		rawResultContent[pos++] = val2Char(((unsigned char)resultContent[i]) & 0x0F);	
	}
	return pos;
}

size_t dumpString(const char* start, const char* end, const int scale, std::string& content, size_t pos)
{
	unsigned short result = 0;
	for(int i = 0; i < end-start; i++) {
		result = result*scale + char2Val(start[i]);
	}
	content[pos] = *((char*)&result);
	content[pos+1] = *((char*)&result + 1);
	return 2;
}

unsigned char* proprocess(unsigned char* str) {
	size_t length = strlen((const char*)str);
	unsigned char* ret = (unsigned char*)malloc((sizeof(unsigned char)*length/6) + 1);
	memset(ret, 0, (sizeof(unsigned char)*length/6) + 1);

	size_t retpos = 0;
	for(size_t i = 0; i + 5 < length; i += 6) {
		if(str[i] == ' ') {
			i -= 5;
			continue;
		}
		bool isLegal = (str[i] == '0' && (str[i+1] == 'x' || str[i+1] == 'X') && isDigit(str[i+2]) && isDigit(str[i+3]) && isDigit(str[i+4]) && isDigit(str[i+5]));
		if(!isLegal) break;

		ret[retpos++] = (char2Val(str[i+4]) << 4) | char2Val(str[i+5]);
		ret[retpos++] = (char2Val(str[i+2]) << 4) | char2Val(str[i+3]);
	}
	free(str);
	return ret;
}


static void show_code(unsigned char* str, const char* fromcode, const char* toCode) {
	printf("%s:\n", toCode);
	//将content中的内容转换
	size_t contentLen = strlen((const char*)str);
	size_t ResultLen = contentLen*4;
	size_t savedLen = contentLen*4;

	std::string resultContent;
	resultContent.resize(ResultLen);

	iconv_t translate = iconv_open(toCode,fromcode);
	assert(translate != (iconv_t)-1);
	char* fromContent = (char*)str;
	char* toContent = &resultContent[0];
	iconv(translate, &fromContent, &contentLen, &toContent, &ResultLen);//pdes会自动加
	if(contentLen != 0)
		return ;

	resultContent.resize(savedLen - ResultLen);
	printf("\tresultContent = %s\n", resultContent.c_str());

	//将结果result的内容在内存中的样子，保存到字符串中。
	std::string rawResultContent;
	rawResultContent.resize(resultContent.length()*8);
	size_t rawResultLen = disDump(resultContent, rawResultContent, " 0x", 3);//按照unsigned short来读的样子
	rawResultContent.resize(rawResultLen);
	printf("\trawResultContent = %s\n\n", rawResultContent.c_str());
}

void showall(const char* fromcode, unsigned char* str) {
	printf("content = %s\n\n",(char*)str);

	show_code(str, fromcode, "utf8");
	show_code(str, fromcode, "gb18030");
	show_code(str, fromcode, "utf16");
	show_code(str, fromcode, "unicode");

	printf("===================================================\n");
}



int main(int ac, char** av)
{
	if(ac != 3) {
		printf("Usage: ./encoding raw 你好啊\n");
		printf("Usage: ./encoding gbk 0x23af0x33420x1222\n");
		printf("Usage: ./encoding utf8 0x23af0x33420x1222\n");
		return 0;
	}

	const char* encodeing = av[1];

	unsigned char* str = (unsigned char*)strdup(av[2]);
	char* fromcode = strdup(av[1]);
	const bool israw = (strcmp(fromcode, "raw") == 0);
	const bool ischeckraw = (strcmp(fromcode, "checkraw") == 0);
	if(!israw && !ischeckraw) {
		str = proprocess(str);
	} else {
		free(fromcode);
		fromcode = strdup("gb18030");
	}

	showall(fromcode, str);

	if(ischeckraw) {
		showall("utf8", str);
		showall("unicode", str);
	}


/*


	std::string rawContent(av[6]);
	std::string content;
	content.resize(rawContent.length());
	
//将字符的内容整成内存存放的样子，放入content中。
	size_t contentLen;
	if(scale != 0) {
		size_t pos = rawContent.find(separate, 0);
		size_t next;
		size_t i = 0;
		while((next = rawContent.find(separate, pos + 1)) != std::string::npos) {
			pos += separateLen;
			i += dumpString(&rawContent[pos], &rawContent[next], scale, content, i);
			pos = next;
		}
		i += dumpString(&rawContent[pos+separateLen], &rawContent[rawContent.length()], scale, content, i);
		contentLen = i;
		content.resize(contentLen);
	} else {
		content.assign(rawContent);
		contentLen = content.length();
	}

	rawContent.resize(content.length()*8);
	size_t rawLen = disDump(content, rawContent, outSeparate, outSeparateLen);
	rawContent.resize(rawLen);
	printf("rawContent = %s\n",rawContent.c_str());


//将content中的内容转换
	std::string resultContent;
	size_t ResultLen = contentLen*4;
	size_t savedLen = contentLen*4;
	resultContent.resize(ResultLen);

	iconv_t translate = iconv_open(toCode,fromCode);
	assert(translate != (iconv_t)-1);
	char* fromContent = &content[0];
	char* toContent = &resultContent[0];
	iconv(translate, &fromContent, &contentLen, &toContent, &ResultLen);//pdes会自动加
	if(contentLen != 0)
		return -1;

	resultContent.resize(savedLen - ResultLen);
	printf("resultContent = %s\n",resultContent.c_str());

//将结果result的内容在内存中的样子，保存到字符串中。
	std::string rawResultContent;
	rawResultContent.resize(resultContent.length()*8);
	size_t rawResultLen = disDump(resultContent, rawResultContent, outSeparate, outSeparateLen);
	rawResultContent.resize(rawResultLen);
	printf("rawResultContent = %s\n",rawResultContent.c_str());

*/

	return 0;
}
