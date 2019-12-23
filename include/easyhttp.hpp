#pragma once

#include <map>
#include <iterator> 
#include <curl/curl.h>
#include <sstream>
#include <chrono>


namespace easyhttp {

	enum class HttpRequestType { get, post };
	enum class RequestError { none, timeout, socket_error, error_misc };
	
	struct BasicAuthentication {
		std::string username;
		std::string password;
	};

	struct HttpResponse {
		RequestError error;
		std::string response_code;
		std::string content;
	};

	


	class Parameters {

	public:
		Parameters() {}

		explicit Parameters(const std::initializer_list<std::pair<std::string, std::string>>& list) {
			for (auto itr = list.begin(); itr != list.end(); itr++) {
				if (!itr->first.empty()) {
					items_[itr->first] = itr->second;
				}
			}
		}


		explicit Parameters(const std::pair<std::string, std::string>& x) {
			items_[x.first] = x.second;
		}

		explicit Parameters(const std::map<std::string, std::string> x) : items_{ x } {}

		void add(std::pair<std::string, std::string> p) {
			if (!p.first.empty()) {
				items_[p.first] = p.second;
			}
		}

		void remove(std::string key) {
			items_.erase(key);
		}

		size_t size() {
			return items_.size();
		}

		std::string get_parameter(std::string key) {
			return (items_.find(key) == items_.end()) ? "" : items_[key];
		}

		void clear() {
			items_.clear();
		}

	protected:
		std::map<std::string, std::string> items_;
	};


	class UrlParameters : public Parameters {
	public:
		UrlParameters() : Parameters() {}

		explicit UrlParameters(std::initializer_list<std::pair<std::string, std::string>> list)
			: Parameters(list) {}

		explicit UrlParameters(const std::pair<std::string, std::string>& x)
			: Parameters(x) {}

		explicit UrlParameters(const std::map<std::string, std::string>& x)
			: Parameters(x) {}


		std::string get_string() {
			if (str_.empty()) {
				encode();
				return str_;
			}

			return str_;
		}
		std::string get_encoded_string() {
			if (encoded_str_.empty()) {
				return encode();
			}

			return encoded_str_;
		}

		std::string encode() {

			if (items_.size() == 0) {
				return "";
			}

			str_.clear();

			for (auto& [k, v] : items_) {
				str_ += ("&" + k + "=" + v);
				encoded_str_ += ("&" + url_escape_str(std::string(k)) + "=" + url_escape_str(std::string(v)));
			}

			str_.erase(0, 1);
			encoded_str_.erase(0, 1);

			return encoded_str_;
		}

	private:
	
		std::string url_escape_str(std::string& orig) {

			// Think about a possible try catch here
			// Technically, std::strings ctor can throw
			char *res = curl_easy_escape(nullptr, orig.c_str(), orig.length());

			std::string escaped_str = std::string(res);
			curl_free(res);
			return escaped_str;
		}

		std::string str_;
		std::string encoded_str_;

	};

	class Headers : public Parameters {
	public:
		Headers() : Parameters() {}

		explicit Headers(std::initializer_list<std::pair<std::string, std::string>> list)
			: Parameters(list) {}

		explicit Headers(const std::pair<std::string, std::string>& x)
			: Parameters(x) {}

		explicit Headers(const std::map<std::string, std::string>& x)
			: Parameters(x) {}

	};

	struct RequestConfig {
		std::string url;
		UrlParameters params;
		Headers headers;
		BasicAuthentication auth;
		std::chrono::seconds timeout_sec;
	};

	namespace{

		size_t http_request_impl_response_write(char* ptr, size_t size, size_t numb, void* ud) {
			size_t response_size = size * numb;
			std::stringstream* ss = (std::stringstream*)ud;
			ss->write(ptr, response_size);
			return response_size;
		}

	}

	class Request {
	public:

		HttpResponse post(RequestConfig& c) {
			HttpResponse resp = http_request_impl(HttpRequestType::post, c);
			return resp;
		}

		HttpResponse get(RequestConfig& c) {
			HttpResponse resp = http_request_impl(HttpRequestType::get, c);
			return resp;
		}

	private:
		HttpResponse http_request_impl(const HttpRequestType r, RequestConfig& c) {

			CURL* curl;
			curl_global_init(CURL_GLOBAL_ALL);
			curl = curl_easy_init();

			std::stringstream response_stream;
			HttpResponse resp;

			if (r == HttpRequestType::post) {
				curl_easy_setopt(curl, CURLOPT_POST, 1);
			}

			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, static_cast<long>(c.timeout_sec.count()));
			curl_easy_setopt(curl, CURLOPT_URL, c.url.c_str());

			if (c.params.size() > 0) {
				std::string temp = c.params.get_encoded_string();
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, temp.c_str());
			}

			else {
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
			}

			if (!c.auth.username.empty() && !c.auth.password.empty()) {
				curl_easy_setopt(curl, CURLOPT_USERNAME, c.auth.username.c_str());
				curl_easy_setopt(curl, CURLOPT_PASSWORD, c.auth.password.c_str());
			}

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_request_impl_response_write);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_stream);

			CURLcode res = curl_easy_perform(curl);

			auto http_code = 0L;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
			curl_easy_cleanup(curl);

			if (res == CURLE_OK) {
				resp.content = response_stream.str();
				resp.error = RequestError::none;
				resp.response_code = std::to_string(http_code);
			}

			else if (res == CURLE_OPERATION_TIMEDOUT) {
				resp.content = "Operation Timed out.";
				resp.error = RequestError::timeout;
				resp.response_code = -1;
			}

			else {
				resp.content = "Request encountered error: " + std::string(curl_easy_strerror(res));
				resp.error = RequestError::error_misc;
				resp.response_code = -1;
			}


			return resp;
			
		}

	};
};
