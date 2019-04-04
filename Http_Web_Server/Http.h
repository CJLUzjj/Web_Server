#pragma once

#include "Channel.h"
#include "EventLoop.h"

#include <map>
#include <unordered_map>

/*
HTTP协议解析使用大量的状态机
使用状态机的理由我个人的理解
一是方便解析便于分析
二是当出现数据没受完全时可以保持原有的状态等到再接收到数据后再执行原有的状态执行
*/
enum Process_State
{
	STATE_REQUEST_LINE = 1,
	STATE_REQUEST_HEAD,
	STATE_REQUEST_BODY,
	STATE_ANALYSIS,
	STATE_FINISH
};

enum Connection_State
{
	CONNECTED = 1,
	DISCONNECTING,
	DISCONNECTED,
};

enum Request_Line_State
{
	PARSE_LINE_AGAIN = 1,
	PARSE_LINE_ERROR,
	PARSE_LINE_SUCCESS
};

enum Request_Head_State
{
	PARSE_HEADER_AGAIN = 1,
	PARSE_HEADER_ERROR,
	PARSE_HEADER_SUCCESS
};

enum Parse_Head_State
{
	H_START = 0,
	H_KEY,
	H_COLON,
	H_SPACES_AFTER_COLON,
	H_VALUE,
	H_CR,
	H_LF,
	H_END_CR,
	H_END_LF
};

enum AnalysisState
{
	ANALYSIS_SUCCESS = 1,
	ANALYSIS_ERROR
};

enum HttpMethod
{
	METHOD_POST = 1,
	METHOD_GET,
	METHOD_HEAD
};

enum HttpVersion
{
	Version10 = 1,
	Version11
};

class MimeType
{
private:
	static void init();
	static std::unordered_map<std::string, std::string> mime;
	MimeType();
	MimeType(const MimeType &m);

public:
	static std::string getMime(const std::string &suffix);

private:
	static pthread_once_t once_control;
};

class Http: public std::enable_shared_from_this<Http>
{
public:
	Http(EventLoop* loop, int fd);
	~Http();
	void init();
	Channel* getchannel()
	{
		return http_channel_;
	}

private:

	Request_Line_State parse_request_line();
	Request_Head_State parse_request_head();
	AnalysisState request_Analysis();

	void handleread();
	void handleError(int err_num, std::string err_msg);
	void handleWrite();
	void handleConn();
	void handleClose();
	void reset();
	EventLoop* loop_;
	const int http_fd_;
	Channel* http_channel_;

	std::string read_buf_;
	std::string write_buf_;
	std::string request_line_;

	Connection_State connect_state_;
	Process_State process_state_;
	Parse_Head_State head_state_;

	bool error_;
	bool keepalive_;
	HttpMethod method_;
	std::string URL_;
	HttpVersion HTTP_version_;
	std::map<std::string, std::string> headers_;
};

