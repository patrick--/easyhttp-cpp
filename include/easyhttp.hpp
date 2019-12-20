#pragma once

#include <map>
#include <iterator> 
#include <curl/curl.h>


namespace easyhttp {


	enum class RequestError {none, timeout, socket_error};
	

	struct BasicAuthentication {
		std::string username;
		std::string password;
	};

	struct HttpResponse {
		std::string response_code;
		std::string content;
	};


	class Parameters {

	public:
		Parameters() {}

		Parameters(std::initializer_list<std::pair<std::string, std::string>> list) {
			for (auto itr = list.begin(); itr != list.end(); itr++) {
				if (!itr->first.empty()) {
					items_[itr->first] = itr->second;
				}
			}
		}


		Parameters(std::pair<std::string, std::string>& x) {
			items_[x.first] = x.second;
		}

		Parameters(std::map<std::string, std::string> x) : items_{ x } {}

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

		UrlParameters(std::initializer_list<std::pair<std::string, std::string>> list)
			: Parameters(list) {}

		UrlParameters(std::pair<std::string, std::string>& x)
			: Parameters(x) {}

		UrlParameters(std::map<std::string, std::string> x)
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

		Headers(std::initializer_list<std::pair<std::string, std::string>> list)
			: Parameters(list) {}

		Headers(std::pair<std::string, std::string>& x)
			: Parameters(x) {}

		Headers(std::map<std::string, std::string> x)
			: Parameters(x) {}

	};

	class Request {

	
	private:
		std::string url_;
		Parameters& params_;


	};

}
