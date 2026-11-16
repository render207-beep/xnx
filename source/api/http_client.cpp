#include "api/http_client.hpp"
#include <curl/curl.h>

static size_t writeCb(char* p, size_t s, size_t n, void* ud) {
    static_cast<std::string*>(ud)->append(p, s * n);
    return s * n;
}

HttpResponse HttpClient::get(const std::string& url,
                             const std::map<std::string, std::string>& headers) {
    HttpResponse r;
    CURL* c = curl_easy_init();
    if (!c) { r.error = "curl_easy_init failed"; return r; }

    curl_slist* hl = nullptr;
    for (auto& [k, v] : headers) hl = curl_slist_append(hl, (k + ": " + v).c_str());

    curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, hl);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, writeCb);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &r.body);
    curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(c, CURLOPT_TIMEOUT, 20L);
    curl_easy_setopt(c, CURLOPT_ACCEPT_ENCODING, "");
    if (insecure) {
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYHOST, 0L);
    } else {
        // Pon cacert.pem en resources/ (se monta en romfs:/)
        curl_easy_setopt(c, CURLOPT_CAINFO, "romfs:/cacert.pem");
    }

    CURLcode rc = curl_easy_perform(c);
    if (rc != CURLE_OK) r.error = curl_easy_strerror(rc);
    curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &r.status);
    curl_slist_free_all(hl);
    curl_easy_cleanup(c);
    return r;
}
