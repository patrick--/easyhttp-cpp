# easyhttp-cpp

### Overview
The goal of this library was to have a minimal CURL wrapper implementation that allowed users to make HTTP requests through configurable and reusable objects. These configurable requests could be individual HTTP calls that are responsible for interacting with different external RESTful API endpoints or services. 

Additionally, basing it on CURL makes keeps it lightweight and fairly straghtforward to extend or modify. 

---

### Functionality
##### `HTTP GET`

| Description  |Supported 
|--------------|:-:|
| Query Parameters |   :heavy_check_mark:      |
| Custom Header | :heavy_check_mark:    |  
| Custom Timeout  | :heavy_check_mark:   |   
| HTTP Basic Authentication  | :heavy_check_mark:   |   

##### `HTTP POST`

| Description  |Supported 
|--------------|:-:|
| Query Parameters |   :heavy_check_mark:      |
| Custom Header | :heavy_check_mark:    |  
| Custom Timeout  | :heavy_check_mark:   |   
| HTTP Basic Authentication  | :heavy_check_mark:   |  

##### `HTTP PUT` :x: 
*In Progress*

##### `HTTP DELETE`:x: 
*In Progress*

##### `HTTP PATCH`:x: 
*In Progress*

---
### Usage

Getting started is as easy as including `easyhttp.hpp` in your source file.

You probably want to create a `RequestConfig` object to store all relevant settings for your particular HTTP request:

```cpp
struct RequestConfig {
  std::string url;
  UrlParameters params;
  Headers headers;
  BasicAuthentication auth;
  std::chrono::seconds timeout_sec;
};
  ```

#### Making a basic `GET` request

Explicit construction of a request configuration object showing available settings:

```cpp
Request req = Request();
RequestConfig config = { "http://postman-echo.com/get", UrlParameters(), Headers(), BasicAuthentication(), std::chrono::seconds(1) };
RequestResponse resp = req.get(config);
std::cout << "Status: " << resp.status << " Content: " << resp.content << "\n";
```

Implict `RequestConfig` object with no settings besides URL and timeout of 3 seconds

```cpp
Request req = Request();
RequestConfig config = { "http://google.com", {}, {}, {}, std::chrono::seconds(3) };
RequestResponse resp = req.get(config);

```

`RequestConfig` with URL paramaters `foo:bar` and timeout of 1 second
```cpp
RequestConfig r = { "http://postman-echo.com/get", {{"foo","bar"}}, {}, {}, std::chrono::seconds(1)};
```



