#pragma once
#include <map>
#include <string>

struct HttpResponse {
    long status = 0;
    std::string body;
    std::string error;
};

class HttpClient {
public:
    explicit HttpClient(bool insecureTls = false) : insecure(insecureTls) {}
    HttpResponse get(const std::string& url,
                     const std::map<std::string, std::string>& headers = {});
private:
    bool insecure;
};
