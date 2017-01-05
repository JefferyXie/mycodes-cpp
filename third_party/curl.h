#ifndef CURL_H
#define CURL_H

#include "../main/header.h"
#include "base64.h"
#include <curl/curl.h>

struct response_t {
    char*   data;
    size_t  size;
};

size_t
curl_response_callback (void* contents, size_t size, size_t nmemb, void* user_data)
{
    size_t realsize = size * nmemb;
    struct response_t* response = (struct response_t*)user_data;

    response->data = static_cast<char*>(realloc(response->data, response->size + realsize + 1));
    if(response->data == NULL) {
        /* out of memory! */ 
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(response->data[response->size]), contents, realsize);
    response->size += realsize;
    response->data[response->size] = 0;

    return realsize;
}

void
curl_http_post (const std::string& url, const std::string& header, const std::string& data)
{
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */ 
    curl = curl_easy_init();
    if (curl)
    {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        /* set custom HTTP headers */
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by itself */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)data.length());

        /* send response data to a callback function  */ 
        response_t response;
        /* will grow as needed by realloc */
        response.data = static_cast<char*>(malloc(1));
        response.size = 0;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_response_callback);
        /* pass our 'response' struct to the callback function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

        /* set verbose mode on */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        /* include header into the body output */
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);

        /* Check for errors */ 
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        else
        {
            printf("Response: %s\n", response.data);
            free(response.data);
        }

        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void
Run_curl () {
    std::string auth_url = "https://id.ttstage.com/oauth/token";
    std::string idsecret = "b7b9974e8e2244e387ab64cadd0d667a:830b5b391b474a4780a8697d0396a27762aa40367e8549578ae71149df8c78c5";
    auto encoded_secret = base64Encode(idsecret);
    auto decoded_secret = base64Decode(encoded_secret);
    printf("id_secret: %s\n", idsecret.c_str());
    printf("encoded_secret: %s\n", encoded_secret.c_str());
    printf("decoded_secret: %s\n", decoded_secret.c_str());

    auto encoded_secret_nolib = base64_encode(idsecret);
    auto decoded_secret_nolib = base64_decode(encoded_secret_nolib);
    printf("encoded_secret_nolib: %s, %d\n", encoded_secret_nolib.c_str(), encoded_secret_nolib == encoded_secret);
    printf("decoded_secret_nolib: %s, %d\n", decoded_secret_nolib.c_str(), decoded_secret_nolib == decoded_secret);

    std::string data = "grant_type=password&username=jeffery.xie@tradingtechnologies.com&password=Learnpython1";
    std::string header = "Authorization: Basic ";
    header += encoded_secret;
    curl_http_post(auth_url, header, data);
}

#endif

