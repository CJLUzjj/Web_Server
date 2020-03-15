//by Zhujunjie
#include "Http.h"
#include "Server.h"
#include "Operation.h"

#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <memory>

#include "Logging.h"

pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::mime;

const int DEFAULT_EXPIRED_TIME = 2000;
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;

char favicon[555] = {
  '\x89', 'P', 'N', 'G', '\xD', '\xA', '\x1A', '\xA',
  '\x0', '\x0', '\x0', '\xD', 'I', 'H', 'D', 'R',
  '\x0', '\x0', '\x0', '\x10', '\x0', '\x0', '\x0', '\x10',
  '\x8', '\x6', '\x0', '\x0', '\x0', '\x1F', '\xF3', '\xFF',
  'a', '\x0', '\x0', '\x0', '\x19', 't', 'E', 'X',
  't', 'S', 'o', 'f', 't', 'w', 'a', 'r',
  'e', '\x0', 'A', 'd', 'o', 'b', 'e', '\x20',
  'I', 'm', 'a', 'g', 'e', 'R', 'e', 'a',
  'd', 'y', 'q', '\xC9', 'e', '\x3C', '\x0', '\x0',
  '\x1', '\xCD', 'I', 'D', 'A', 'T', 'x', '\xDA',
  '\x94', '\x93', '9', 'H', '\x3', 'A', '\x14', '\x86',
  '\xFF', '\x5D', 'b', '\xA7', '\x4', 'R', '\xC4', 'm',
  '\x22', '\x1E', '\xA0', 'F', '\x24', '\x8', '\x16', '\x16',
  'v', '\xA', '6', '\xBA', 'J', '\x9A', '\x80', '\x8',
  'A', '\xB4', 'q', '\x85', 'X', '\x89', 'G', '\xB0',
  'I', '\xA9', 'Q', '\x24', '\xCD', '\xA6', '\x8', '\xA4',
  'H', 'c', '\x91', 'B', '\xB', '\xAF', 'V', '\xC1',
  'F', '\xB4', '\x15', '\xCF', '\x22', 'X', '\x98', '\xB',
  'T', 'H', '\x8A', 'd', '\x93', '\x8D', '\xFB', 'F',
  'g', '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f', 'v',
  'f', '\xDF', '\x7C', '\xEF', '\xE7', 'g', 'F', '\xA8',
  '\xD5', 'j', 'H', '\x24', '\x12', '\x2A', '\x0', '\x5',
  '\xBF', 'G', '\xD4', '\xEF', '\xF7', '\x2F', '6', '\xEC',
  '\x12', '\x20', '\x1E', '\x8F', '\xD7', '\xAA', '\xD5', '\xEA',
  '\xAF', 'I', '5', 'F', '\xAA', 'T', '\x5F', '\x9F',
  '\x22', 'A', '\x2A', '\x95', '\xA', '\x83', '\xE5', 'r',
  '9', 'd', '\xB3', 'Y', '\x96', '\x99', 'L', '\x6',
  '\xE9', 't', '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',
  '\xA7', '\xC4', 'b', '1', '\xB5', '\x5E', '\x0', '\x3',
  'h', '\x9A', '\xC6', '\x16', '\x82', '\x20', 'X', 'R',
  '\x14', 'E', '6', 'S', '\x94', '\xCB', 'e', 'x',
  '\xBD', '\x5E', '\xAA', 'U', 'T', '\x23', 'L', '\xC0',
  '\xE0', '\xE2', '\xC1', '\x8F', '\x0', '\x9E', '\xBC', '\x9',
  'A', '\x7C', '\x3E', '\x1F', '\x83', 'D', '\x22', '\x11',
  '\xD5', 'T', '\x40', '\x3F', '8', '\x80', 'w', '\xE5',
  '3', '\x7', '\xB8', '\x5C', '\x2E', 'H', '\x92', '\x4',
  '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g', '\x98',
  '\xE9', '6', '\x1A', '\xA6', 'g', '\x15', '\x4', '\xE3',
  '\xD7', '\xC8', '\xBD', '\x15', '\xE1', 'i', '\xB7', 'C',
  '\xAB', '\xEA', 'x', '\x2F', 'j', 'X', '\x92', '\xBB',
  '\x18', '\x20', '\x9F', '\xCF', '3', '\xC3', '\xB8', '\xE9',
  'N', '\xA7', '\xD3', 'l', 'J', '\x0', 'i', '6',
  '\x7C', '\x8E', '\xE1', '\xFE', 'V', '\x84', '\xE7', '\x3C',
  '\x9F', 'r', '\x2B', '\x3A', 'B', '\x7B', '7', 'f',
  'w', '\xAE', '\x8E', '\xE', '\xF3', '\xBD', 'R', '\xA9',
  'd', '\x2', 'B', '\xAF', '\x85', '2', 'f', 'F',
  '\xBA', '\xC', '\xD9', '\x9F', '\x1D', '\x9A', 'l', '\x22',
  '\xE6', '\xC7', '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15',
  '\x90', '\x7', '\x93', '\xA2', '\x28', '\xA0', 'S', 'j',
  '\xB1', '\xB8', '\xDF', '\x29', '5', 'C', '\xE', '\x3F',
  'X', '\xFC', '\x98', '\xDA', 'y', 'j', 'P', '\x40',
  '\x0', '\x87', '\xAE', '\x1B', '\x17', 'B', '\xB4', '\x3A',
  '\x3F', '\xBE', 'y', '\xC7', '\xA', '\x26', '\xB6', '\xEE',
  '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
  '\xA', '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X', '\x0',
  '\x27', '\xEB', 'n', 'V', 'p', '\xBC', '\xD6', '\xCB',
  '\xD6', 'G', '\xAB', '\x3D', 'l', '\x7D', '\xB8', '\xD2',
  '\xDD', '\xA0', '\x60', '\x83', '\xBA', '\xEF', '\x5F', '\xA4',
  '\xEA', '\xCC', '\x2', 'N', '\xAE', '\x5E', 'p', '\x1A',
  '\xEC', '\xB3', '\x40', '9', '\xAC', '\xFE', '\xF2', '\x91',
  '\x89', 'g', '\x91', '\x85', '\x21', '\xA8', '\x87', '\xB7',
  'X', '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N', 'N',
  'b', 't', '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
  '\xEC', '\x86', '\x2', 'H', '\x26', '\x93', '\xD0', 'u',
  '\x1D', '\x7F', '\x9', '2', '\x95', '\xBF', '\x1F', '\xDB',
  '\xD7', 'c', '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF',
  '\x22', 'J', '\xC3', '\x87', '\x0', '\x3', '\x0', 'K',
  '\xBB', '\xF8', '\xD6', '\x2A', 'v', '\x98', 'I', '\x0',
  '\x0', '\x0', '\x0', 'I', 'E', 'N', 'D', '\xAE',
  'B', '\x60', '\x82',
};

void MimeType::init()
{
	mime[".html"] = "text/html";
	mime[".avi"] = "video/x-msvideo";
	mime[".bmp"] = "image/bmp";
	mime[".c"] = "text/plain";
	mime[".doc"] = "application/msword";
	mime[".gif"] = "image/gif";
	mime[".gz"] = "application/x-gzip";
	mime[".htm"] = "text/html";
	mime[".ico"] = "image/x-icon";
	mime[".jpg"] = "image/jpeg";
	mime[".png"] = "image/png";
	mime[".txt"] = "text/plain";
	mime[".mp3"] = "audio/mp3";
	mime[".pdf"] = "application/pdf";
	mime["default"] = "text/html";
}

std::string MimeType::getMime(const std::string &suffix)
{
	pthread_once(&once_control, MimeType::init);
	if (mime.find(suffix) == mime.end())
		return mime["default"];
	else
		return mime[suffix];
}

Http::Http(EventLoop* loop, int fd)
	:loop_(loop),
	 http_fd_(fd),
	 http_channel_(new Channel(loop_, http_fd_)),
	 connect_state_(CONNECTED),
	 process_state_(STATE_REQUEST_LINE),
	 head_state_(H_START),
	 keepalive_(false),
	 error_(false)
{
	//printf("http_fd_ is %d\n", http_fd_);
}


Http::~Http()
{
	//std::cout << "Http destroy" << std::endl;
	std::cout<<"free http:"<<http_fd_<<std::endl;
}

void Http::init()
{
	http_channel_->enableReading();
	http_channel_->setReadCallBack(std::bind(&Http::handleread, this));
	http_channel_->setWriteCallBack(std::bind(&Http::handleWrite, this));
	http_channel_->setConnCallBack_(std::bind(&Http::handleConn, this));
	printf("A new Http !http_fd_ is %d\n", http_fd_);
}

void Http::handleread()
{
	//printf("http_fd_ is %d\n", this->http_fd_);
	//int n = read(this->http_fd_, read_buf_, 1024 * 64);
	//if (n > 0) {
	//	write(stdout_fileno, read_buf_, sizeof read_buf_);
	//}
	//else {
	//	char err[1024];
	//	bzero(err, 1024);
	//	snprintf(err, 1024, "oops read error! %s\n", strerror(errno));
	//	write(stdout_fileno, err, 1024);
	//	
	//}
	//bzero(read_buf_, sizeof read_buf_);


	std::cout <<"handleread:" << http_fd_ << std::endl;
	do
	{
		//int readnum = 0;
		//int readsum = 0;
		//while (true)
		//{
		//	char buf[4096];
		//	int readnum = read(http_fd_, buf, 4096);
		//	if (readnum < 0) {
		//		if (errno == EINTR) {
		//			continue;
		//		}
		//		else if (errno == EAGAIN) {
		//			break;
		//		}
		//		else
		//		{
		//			perror("read error!");
		//			//错误处理
		//			error_ = true;
		//			handleError(400, "Bad Request");
		//			break;
		//		}
		//	}
		//	else if (readnum == 0) {
		//		printf("readnum = %d\n", readnum);	//读0字节，对端关闭连接
		//		connect_state_ = DISCONNECTING;
		//		break;
		//	}
		//	readsum += readnum;
		//	read_buf_ += std::string(buf, readnum);
		//}

		//if (connect_state_ == DISCONNECTING || error_ == true) {
		//	break;
		//}

		//std::cout << "readsum = " << readsum << std::endl;

		bool zero = false;
		int read_num = readn(http_fd_, read_buf_, zero);
		//LOG << "Request: " << read_buf_;
		std::cout<<"Request:"<<std::endl<<read_buf_<<std::endl;
		if (connect_state_ == DISCONNECTING)
		{
			read_buf_.clear();
			break;
		}
		//cout << inBuffer_ << endl;
		if (read_num < 0)
		{
			error_ = true;
			//handleError(400, "Bad Request");
			break;
		}
		// else if (read_num == 0)
		// {
		//     error_ = true;
		//     break; 
		// }
		else if (zero)
		{
			// 有请求出现但是读不到数据，可能是Request Aborted，或者来自网络的数据没有达到等原因
			// 最可能是对端已经关闭了，统一按照对端已经关闭处理
			//error_ = true;
			connect_state_ = DISCONNECTING;
			if (read_num == 0)
			{
				//error_ = true;
				std::cout << "readnum == 0" << std::endl;
				break;
			}
		}

		if (connect_state_ == DISCONNECTED) {
			read_buf_.clear();
			break;
		}

		if (process_state_ == STATE_REQUEST_LINE) {
			Request_Line_State flag = parse_request_line();
			//std::cout << "parse_request_line" << std::endl;
			if (flag == PARSE_LINE_AGAIN) {
				break;
			}
			else if(flag == PARSE_LINE_ERROR)
			{
				perror("2");
				LOG << "FD = " << http_fd_ << "," << read_buf_ << "******";
				read_buf_.clear();
				error_ = true;
				handleError(400, "Bad Request");
				break;
			}
			else
			{
				process_state_ = STATE_REQUEST_HEAD;
			}
		}

		if (process_state_ == STATE_REQUEST_HEAD) {
			Request_Head_State flag = parse_request_head();
			//std::cout << "parse_request_head" << std::endl;
			if (flag == PARSE_HEADER_AGAIN) {
				break;
			}
			else if (flag == PARSE_HEADER_ERROR)
			{
				perror("3");
				error_ = true;
				handleError(400, "Bad Request");
				break;
			}
			if (method_ == METHOD_POST)
			{
				process_state_ = STATE_REQUEST_BODY;
			}
			else
			{
				process_state_ = STATE_ANALYSIS;
			}
		}

		if (process_state_ == STATE_REQUEST_BODY) {
			int content_length = -1;
			if (headers_.find("Content-length") != headers_.end()) {
				content_length = std::stoi(headers_["Content-length"]);
			}
			else
			{
				error_ = true;
				handleError(400, "Bad Request: Lack of argument (Content-length)");
				break;
			}
			if (static_cast<int>(read_buf_.size()) < content_length)
				break;
			process_state_ = STATE_ANALYSIS;
		}

		if (process_state_ == STATE_ANALYSIS) {
			AnalysisState flag = request_Analysis();
			//std::cout << "request_Analysis" << std::endl;
			if (flag == ANALYSIS_SUCCESS)
			{
				process_state_ = STATE_FINISH;
				break;
			}
			else
			{
				error_ = true;
				break;
			}
		}

	} while (false);	//while主要用于break跳转

	if (!error_) {
		if (write_buf_.size() > 0)
		{
			handleWrite();
		}
		if (!error_ && process_state_ == STATE_FINISH)
		{
			this->reset();
			if (read_buf_.size() > 0)
			{
				if (connect_state_ != DISCONNECTING)
					handleread();
			}

		}else if(!error_ && connect_state_ == DISCONNECTED){
			http_channel_->disableAll();
		}
		//else if (!error_ && connect_state_ != DISCONNECTED)
			//events_ |= EPOLLIN;
	}
}

void Http::reset()
{
	URL_.clear();
	process_state_ = STATE_REQUEST_LINE;
	head_state_ = H_START;
	headers_.clear();
	error_ = false;
		if(time_.lock())
	{
		std::shared_ptr<TimeNode> my_time(time_.lock());
		my_time->clear();
		time_.reset();
	}
}

void Http::handleWrite()
{
	std::cout << "handleWrite" << std::endl;
	if (!error_ && connect_state_ != DISCONNECTED)
	{
		if (writen(http_fd_, write_buf_) < 0)
		{
			perror("writen");
			//events_ = 0;
			error_ = true;
		}
		if (write_buf_.size() > 0){
			http_channel_->enableWriting();
			http_channel_->disableReading();
			std::cout<<"no write"<<std::endl;
		}
		else{
			http_channel_->disableWriting();
		}
	}
}

void Http::handleConn()
{
	if(time_.lock())
	{
		std::shared_ptr<TimeNode> my_time(time_.lock());
		my_time->clear();
		time_.reset();
	}
	if (!error_ && connect_state_ == CONNECTED) {
		if(http_channel_->isWriting()){//没写完的情况
			int timeout = DEFAULT_EXPIRED_TIME;
			if(keepalive_)
				timeout = DEFAULT_KEEP_ALIVE_TIME;
			loop_->add_time(http_channel_, timeout);
		}else if(keepalive_){
			if(!http_channel_->isReading())
				http_channel_->enableReading();
			int timeout = DEFAULT_KEEP_ALIVE_TIME;
			loop_->add_time(http_channel_, timeout);
		}else{
			if(!http_channel_->isReading())
				http_channel_->enableReading();
			int timeout = DEFAULT_KEEP_ALIVE_TIME>>1;
			loop_->add_time(http_channel_, timeout);
		}
	}else if(!error_ && connect_state_ == DISCONNECTING && http_channel_->isWriting()){
		//对端连接关闭且还有数据未发送的情况，暂时不关闭连接，等下次发送完再关闭	
	}
	else
	{
		loop_->queueInLoop(std::bind(&Http::handleClose, shared_from_this()));//share_from_this
	}
}

void Http::handleClose()
{
	//printf("handleclose\n");
	http_channel_->disableAll();
	http_channel_->remove();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	std::shared_ptr<Http> guard(shared_from_this());
	http_channel_->http_info_.reset();
	std::cout << "http_channel_ ptr count is " << http_channel_.use_count() << std::endl;
	std::cout << "Http ptr count is " << guard.use_count() << std::endl;
	close(http_fd_);
}

Request_Line_State Http::parse_request_line()
{

	int loc = read_buf_.find("\r\n", 0);
	if (loc < 0) {
		std::cout << "read need again\n" << std::endl;
		return PARSE_LINE_AGAIN;
	}
	request_line_ = read_buf_.substr(0, loc);


	if (read_buf_.size() > loc + 1)
		read_buf_ = read_buf_.substr(loc + 1);
	else
		read_buf_.clear();


	//std::cout << "request_line:" << request_line_ << std::endl;

	int pos;
	//分析method
	int posGet = request_line_.find("GET");		
	int posPost = request_line_.find("POST");
	int posHead = request_line_.find("HEAD");
	if (posGet >= 0)
	{
		pos = posGet;
		method_ = METHOD_GET;
		//printf("method_ is GET\n");
	}
	else if (posPost >= 0)
	{
		pos = posPost;
		method_ = METHOD_POST;
		//printf("method_ is POST\n");
	}
	else if (posHead >= 0)
	{
		pos = posHead;
		method_ = METHOD_HEAD;
		//printf("method_ is HEAD\n");
	}
	else
	{
		printf("parse method error!\n");
		return PARSE_LINE_ERROR;
	}

	pos = request_line_.find("/", pos);	//分析url
	if (pos < 0) {
		URL_ = "index.html";
		HTTP_version_ = Version11;
		//std::cout << "URL_ is " << URL_ << std::endl;
		//printf("HTTP_version_ is HTTP1.1\n");
		return PARSE_LINE_SUCCESS;
	}
	else {
		int _pos = request_line_.find(" ", pos);
		if (_pos < 0) {
			printf("parse url error!\n");
			return PARSE_LINE_ERROR;
		}
		else
		{
			if (_pos - pos > 1) {
				URL_ = request_line_.substr(pos+1, _pos - pos - 1);
				size_t __pos = URL_.find('?');
				if (__pos > 0) {
					URL_ = URL_.substr(0, __pos);
				}
			}
			else {
				URL_ = "index.html";
			}
		}
		pos = _pos;
	}
	//std::cout << "URL_ is " << URL_ << std::endl;

	//分析http版本号
	pos = request_line_.find("/", pos);
	if (pos < 0) {
		printf("parse http_version error\n");
		return PARSE_LINE_ERROR;
	}
	else
	{
		std::string ver;
		ver = request_line_.substr(pos + 1, 3);
		if (ver == "1.0") {
			HTTP_version_ = Version10;
			//printf("HTTP_version_ is HTTP1.0\n");
		}
		else if(ver == "1.1")
		{
			HTTP_version_ = Version11;
			//printf("HTTP_version_ is HTTP1.1\n");
		}
		else
		{
			printf("parse http_version error\n");
			return PARSE_LINE_ERROR;
		}
	}
	return PARSE_LINE_SUCCESS;
}

Request_Head_State Http::parse_request_head()
{
	std::string& str = read_buf_;
	int key_start = -1, key_end = -1, value_start = -1, value_end = -1;
	int now_read_line_begin = 0;
	bool notFinish = true;
	size_t i = 0;
	for (; i < read_buf_.size() && notFinish; i++) {
		switch (head_state_)
		{
			case H_START:
			{
				if (str[i] == '\n' || str[i] == '\r')
					break;
				head_state_ = H_KEY;
				key_start = i;
				now_read_line_begin = i;
				break;
			}
			case H_KEY:
			{
				if (str[i] == ':')
				{
					key_end = i;
					if (key_end - key_start <= 0)
						return PARSE_HEADER_ERROR;
					head_state_ = H_COLON;
				}
				else if (str[i] == '\n' || str[i] == '\r')
					return PARSE_HEADER_ERROR;
				break;
			}
			case H_COLON:
			{
				if (str[i] == ' ')
				{
					head_state_ = H_SPACES_AFTER_COLON;
				}
				else
					return PARSE_HEADER_ERROR;
				break;
			}
			case H_SPACES_AFTER_COLON:
			{
				head_state_ = H_VALUE;
				value_start = i;
				break;
			}
			case H_VALUE:
			{
				if (str[i] == '\r')
				{
					head_state_ = H_CR;
					value_end = i;
					if (value_end - value_start <= 0)
						return PARSE_HEADER_ERROR;
				}
				else if (i - value_start > 255)
					return PARSE_HEADER_ERROR;
				break;
			}
			case H_CR:
			{
				if (str[i] == '\n')
				{
					head_state_ = H_LF;
					std::string key(str.begin() + key_start, str.begin() + key_end);
					std::string value(str.begin() + value_start, str.begin() + value_end);
					headers_[key] = value;
					now_read_line_begin = i;
				}
				else
					return PARSE_HEADER_ERROR;
				break;
			}
			case H_LF:
			{
				if (str[i] == '\r')
				{
					head_state_ = H_END_CR;
				}
				else
				{
					key_start = i;
					head_state_ = H_KEY;
				}
				break;
			}
			case H_END_CR:
			{
				if (str[i] == '\n')
				{
					head_state_ = H_END_LF;
				}
				else
					return PARSE_HEADER_ERROR;
				break;
			}
			case H_END_LF:
			{
				notFinish = false;
				key_start = i;
				now_read_line_begin = i;
				break;
			}
		}
	}
	if (head_state_ == H_END_LF)
	{
		str = str.substr(i);
		return PARSE_HEADER_SUCCESS;
	}
	str = str.substr(now_read_line_begin);
	return PARSE_HEADER_AGAIN;
}

AnalysisState Http::request_Analysis()
{
	if (method_ == METHOD_POST) {

	}
	else if(method_ == METHOD_GET || method_ == METHOD_HEAD)
	{
		std::string header;
		header += "HTTP/1.1 200 OK\r\n";
		if (headers_.find("Connection") != headers_.end() && (headers_["Connection"] == "Keep-Alive" || headers_["Connection"] == "keep-alive")) {
			keepalive_ = true;
			header += std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=300\r\n";
		}
		int dot_pos = URL_.find('.');
		std::string filetype;
		if (dot_pos < 0)
			filetype = MimeType::getMime("default");
		else
			filetype = MimeType::getMime(URL_.substr(dot_pos));

		filetype += ";charset=utf-8";

		// echo test
		if (URL_ == "hello")
		{
			write_buf_ = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
			return ANALYSIS_SUCCESS;
		}

		if (URL_ == "favicon.ico")
		{
			header += "Content-Type: image/png\r\n";
			header += "Content-Length: " + std::to_string(sizeof favicon) + "\r\n";
			header += "Server: Zhujunjie's Web Server\r\n";

			header += "\r\n";
			write_buf_ += header;
			write_buf_ += std::string(favicon, favicon + sizeof favicon);
			return ANALYSIS_SUCCESS;
		}

		struct stat sbuf;
		URL_.insert(0, "./html/");
		if (stat(URL_.c_str(), &sbuf) < 0)
		{
			header.clear();
			handleError(404, "Not Found!");
			return ANALYSIS_ERROR;
		}
		header += "Content-Type: " + filetype + "\r\n";
		header += "Content-Length: " + std::to_string(sbuf.st_size) + "\r\n";
		header += "Server: ZhuJunjie's Web Server\r\n";
		// 头部结束
		header += "\r\n";
		write_buf_ += header;

		if (method_ == METHOD_HEAD)
			return ANALYSIS_SUCCESS;

		//get
		int src_fd = open(URL_.c_str(), O_RDONLY, 0);
		if (src_fd < 0)
		{
			write_buf_.clear();
			handleError(404, "Not Found!");
			return ANALYSIS_ERROR;
		}
		void *mmapRet = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
		close(src_fd);
		if (mmapRet == (void *)-1)
		{
			munmap(mmapRet, sbuf.st_size);
			write_buf_.clear();
			handleError(404, "Not Found!");
			return ANALYSIS_ERROR;
		}
		char *src_addr = static_cast<char*>(mmapRet);
		write_buf_ += std::string(src_addr, src_addr + sbuf.st_size);
		munmap(mmapRet, sbuf.st_size);
		return ANALYSIS_SUCCESS;
	}
}

void Http::handleError(int err_num, std::string err_msg)
{
	std::cout<<"404 Not Found"<<std::endl;
	err_msg = " " + err_msg;
	char send_buff[4096];
	std::string body_buff, header_buff;
	body_buff += "<html><title>error!</title>";
	body_buff += "<body bgcolor=\"ffffff\">";
	body_buff += std::to_string(err_num) + err_msg;
	body_buff += "<hr><em> ZhuJunjie's Web Server</em>\n</body></html>";

	header_buff += "HTTP/1.1 " + std::to_string(err_num) + err_msg + "\r\n";
	header_buff += "Content-Type: text/html\r\n";
	header_buff += "Connection: Close\r\n";
	header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
	header_buff += "Server: ZhuJunjie's Web Server\r\n";;
	header_buff += "\r\n";

	sprintf(send_buff, "%s", header_buff.c_str());
	writen(http_fd_, send_buff, strlen(send_buff));
	sprintf(send_buff, "%s", body_buff.c_str());
	writen(http_fd_, send_buff, strlen(send_buff));
}
