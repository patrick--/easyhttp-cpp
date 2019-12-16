#define CATCH_CONFIG_MAIN 

#include <catch2/catch.hpp>
#include "easyhttp.hpp"


TEST_CASE("Testing HttpParameters basic initialization given <string,string> map") {
	std::map<std::string, std::string> temp = { {"key1","value1"} };

	HttpParameters h = HttpParameters(temp);
}

TEST_CASE("Testing HttpParameters returns size") {
	std::map<std::string, std::string> temp =
	{
		{ "key1","value1" }, {"key2","value2"},
		{"key3","value3"}, {"key4","value4"}
	};

	HttpParameters h;

	SECTION("Checking that an empty HttpParameters objects returns 0") {
		REQUIRE(h.size() == 0);
	}

	SECTION("Checking  that an HttpParamaters objects given a map with 4 entries returns 4 ") {
		h = HttpParameters(temp);
		REQUIRE(h.size() == 4);
	}
}


TEST_CASE("Testing HttpParameters constructing with initilization lists") {
	HttpParameters h;

	SECTION("Initilizing an empty HttpParameter") {
		REQUIRE(h.size() == 0);
	}

	SECTION("Initilaizing an HttpParameter with a single key value entry") {
		h = HttpParameters({ {"key2","value2"} });
		REQUIRE(h.size() == 1);
	}

	SECTION("Initializing an HttpParameter with a map of key value pairs as initilizer list") {
		h = HttpParameters({ {"key1","value1"}, {"key2","value2"} });
		REQUIRE(h.size() == 2);
	}

	SECTION("Initializing an HttpParameter with a invalid key but valid value") {
		h = HttpParameters({ {"","value1"} });
		REQUIRE(h.size() == 0);
	}

	SECTION("Initializing an HttpParameter with a valid key but invalid value") {
		h = HttpParameters({ {"key1",""} });
		REQUIRE(h.size() == 1);
	}
}

TEST_CASE("Getting parameter values from an HttpParameters object ") {
	HttpParameters h;

	SECTION("Attempting to get a parameter that doesn't exist") {
		REQUIRE(h.get_parameter("doesntexist") == "");
	}

	SECTION("Attempting to get a parameter that exists") {
		h = HttpParameters(std::map<std::string, std::string>{ {"key1", "value1"} });
		REQUIRE(h.get_parameter("key1") == "value1");
	}
}

TEST_CASE("Adding key value pairs to HttpParameter object") {
	HttpParameters h;

	SECTION("Adding a single entry to a blank HttpParameters object") {
		h.add({ "key1","value1" });
		REQUIRE(h.size() == 1);
	}

	SECTION("Adding a duplicate entry to a blank HttpParameters object") {
		h.add({ "key1","value1" });
		h.add({ "key1","value2" });
		REQUIRE(h.size() == 1);
		REQUIRE(h.get_parameter("key1") == "value2");
	}

	SECTION("Adding an entry with a blank key to a HttpParameters object") {
		h.add({ "","value1" });
		REQUIRE(h.size() == 0);
	}
}

TEST_CASE("Removing URL parameters from HttpParameters object") {

	HttpParameters h;

	SECTION("Removing a URL parameter that exists") {
		h.add({ "key1","value1" });
		REQUIRE(h.get_parameter("key1") == "value1");

		h.remove("key1");
		REQUIRE(h.get_parameter("key1") == "");
		REQUIRE(h.size() == 0);
	}
	 
}

TEST_CASE("Removing all parameters") {
	HttpParameters h = HttpParameters();

	SECTION("Removing all parameters from an HttpParameters object with values") {
		h.add({ "key1","value1" });
		h.add({ "key2","value2" });
		REQUIRE(h.size() == 2);

		h.clear();
		REQUIRE(h.size() == 0);
	}

	SECTION("Removing  parameters from an empty HttpParameters object") {
		h.clear();
		REQUIRE(h.size() == 0);
	}
}

TEST_CASE("HttpParameters encoding") {
	INFO("Testing different ways of encoding parameters");

	HttpParameters h = HttpParameters();

	std::map<std::string, std::string> temp_map = std::map<std::string, std::string>{ {"key1", "value1"}, { "key2","value2" } };


	SECTION("Testing encoding result of blank params") {

		REQUIRE(h.encode() == "");
	}

	SECTION("Testing encoding result of populated params") {

		h = HttpParameters({ {"key1","value1"},{"key2","value2"} });
		REQUIRE(h.encode() == "key1=value1&key2=value2");
	}

}