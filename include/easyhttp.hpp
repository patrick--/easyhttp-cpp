#pragma once

#include <map>
#include <iterator> 


struct HttpResponse {
	std::string response_code;
	std::string content; 
};


class HttpParameters {

public:
	HttpParameters() {}

	HttpParameters(std::initializer_list<std::pair<std::string, std::string>> list) {
		for (auto itr = list.begin(); itr != list.end(); itr++) {
			if (!itr->first.empty()) {
				items_[itr->first] = itr->second;
			}
		}
	}
	

	HttpParameters(std::pair<std::string, std::string> &x) {
		items_[x.first] = x.second;
	}

	HttpParameters(std::map<std::string, std::string> x) : items_{ x } {}

	void add(std::pair<std::string, std::string> p) {
		if (!p.first.empty()) {
			items_[p.first] = p.second;
		}
	}

	void erase(std::string key) {
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

	std::string get_encoded() {
		if (str_.empty()) {
			return encode();
		}
		return str_;
	}

	std::string encode() {
		str_.clear();

		for (const auto& [k, v] : items_) {
			str_ += ("&" + k + "=" + v);
		}
		
		str_.erase(0,1);

		return str_;
	}


private: 
	std::map<std::string, std::string> items_;
	std::string str_;

	

};
