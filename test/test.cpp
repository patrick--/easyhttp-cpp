#define CATCH_CONFIG_MAIN 

#include <catch2/catch.hpp>
#include "easyhttp.hpp"

using namespace easyhttp;


TEST_CASE("Testing Parameters basic initialization given <string,string> map") {
	std::map<std::string, std::string> temp = { {"key1","value1"} };

	Parameters h = Parameters(temp);
}

TEST_CASE("Testing Parameters returns size") {
	std::map<std::string, std::string> temp =
	{
		{ "key1","value1" }, {"key2","value2"},
		{"key3","value3"}, {"key4","value4"}
	};

	Parameters h;

	SECTION("Checking that an empty Parameters objects returns 0") {
		REQUIRE(h.size() == 0);
	}

	SECTION("Checking  that an Parameters objects given a map with 4 entries returns 4 ") {
		h = Parameters(temp);
		REQUIRE(h.size() == 4);
	}
}


TEST_CASE("Testing Parameters constructing with initilization lists") {
	Parameters h;

	SECTION("Initilizing an empty Parameters") {
		REQUIRE(h.size() == 0);
	}

	SECTION("Initilaizing an Parameters with a single key value entry") {
		h = Parameters({ {"key2","value2"} });
		REQUIRE(h.size() == 1);
	}

	SECTION("Initializing an Parameters with a map of key value pairs as initilizer list") {
		h = Parameters({ {"key1","value1"}, {"key2","value2"} });
		REQUIRE(h.size() == 2);
	}

	SECTION("Initializing an Parameters with a invalid key but valid value") {
		h = Parameters({ {"","value1"} });
		REQUIRE(h.size() == 0);
	}

	SECTION("Initializing an Parameters with a valid key but invalid value") {
		h = Parameters({ {"key1",""} });
		REQUIRE(h.size() == 1);
	}
}

TEST_CASE("Getting parameter values from an Parameters object ") {
	Parameters h;

	SECTION("Attempting to get a parameter that doesn't exist") {
		REQUIRE(h.get_parameter("doesntexist") == "");
	}

	SECTION("Attempting to get a parameter that exists") {
		h = Parameters(std::map<std::string, std::string>{ {"key1", "value1"} });
		REQUIRE(h.get_parameter("key1") == "value1");
	}
}

TEST_CASE("Adding key value pairs to Parameters object") {
	Parameters h;

	SECTION("Adding a single entry to a blank Parameters object") {
		h.add({ "key1","value1" });
		REQUIRE(h.size() == 1);
	}

	SECTION("Adding a duplicate entry to a blank Parameters object") {
		h.add({ "key1","value1" });
		h.add({ "key1","value2" });
		REQUIRE(h.size() == 1);
		REQUIRE(h.get_parameter("key1") == "value2");
	}

	SECTION("Adding an entry with a blank key to a Parameters object") {
		h.add({ "","value1" });
		REQUIRE(h.size() == 0);
	}
}

TEST_CASE("Removing URL parameters from Parameters object") {

	Parameters h;

	SECTION("Removing a URL parameter that exists") {
		h.add({ "key1","value1" });
		REQUIRE(h.get_parameter("key1") == "value1");

		h.remove("key1");
		REQUIRE(h.get_parameter("key1") == "");
		REQUIRE(h.size() == 0);
	}
	 
}

TEST_CASE("Removing all parameters") {
	Parameters h = Parameters();

	SECTION("Removing all parameters from an Parameters object with values") {
		h.add({ "key1","value1" });
		h.add({ "key2","value2" });
		REQUIRE(h.size() == 2);

		h.clear();
		REQUIRE(h.size() == 0);
	}

	SECTION("Removing  parameters from an empty Parameters object") {
		h.clear();
		REQUIRE(h.size() == 0);
	}
}

TEST_CASE("UrlParameters encoding") {
	INFO("Testing different ways of encoding parameters");

	UrlParameters h = UrlParameters();

	std::map<std::string, std::string> temp_map = std::map<std::string, std::string>{ {"key1", "value1"}, { "key2","value2" } };


	SECTION("Testing encoding result of blank params") {

		REQUIRE(h.encode() == "");
	}

	SECTION("Testing encoding result of populated params") {

		h = UrlParameters({ {"key1","value1"},{"key2","value2"} });
		REQUIRE(h.encode() == "key1=value1&key2=value2");
	}

	SECTION("Testing encoding result of populated params that need escaping") {

		h = UrlParameters({ {"key 1","value 1"},{"key 2","value 2"} });
		REQUIRE(h.encode() == "key%201=value%201&key%202=value%202");
	}

}

TEST_CASE("Making requests") {

	UrlParameters p = UrlParameters();
	Headers h = Headers();
	BasicAuthentication a = BasicAuthentication();
	Request b = Request();
	RequestConfig r = { "http://postman-echo.com/post", UrlParameters({{"foo","bar"}}),Headers(), BasicAuthentication(),  std::chrono::seconds(1) };


	SECTION("Testing post request") {
		HttpResponse resp = b.post(r);

		REQUIRE(resp.response_code == "200");
	}

	SECTION("Testing get request") {
		HttpResponse resp = b.get(r);

		REQUIRE(resp.response_code == "200");
	}

	


}