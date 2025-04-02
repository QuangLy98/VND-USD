#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h> 


size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data) {
    strcat(data, (char *)ptr);
    return size * nmemb;
}

int main() {
    float num, result, exchangeRate = 0;
    char operator;
    char response[1000] = "";

    CURL *curl = curl_easy_init();
    if (curl) {
        const char *url = "https://api.exchangerate-api.com/v4/latest/USD";

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            char *rate_ptr = strstr(response, "\"VND\":");
            if (rate_ptr) {
                sscanf(rate_ptr, "\"VND\":%f", &exchangeRate);
                printf("Tỷ giá hiện tại (USD -> VND): %.2f\n", exchangeRate);
            } else {
                printf("Không tìm thấy tỷ giá VND trong phản hồi API.\n");
                return 1;
            }
        } else {
            printf("Lỗi khi gọi API: %s\n", curl_easy_strerror(res));
            return 1;
        }

        curl_easy_cleanup(curl);
    } else {
        printf("Không thể khởi tạo libcurl.\n");
        return 1;
    }

    printf("Chọn phép tính (V: VND sang USD, U: USD sang VND): ");
    scanf(" %c", &operator);

    printf("Nhập giá trị cần tính: ");
    scanf("%f", &num);

    switch (operator) {
        case 'V':
            result = num / exchangeRate;
            printf("Kết quả: %.2f VND = %.2f USD\n", num, result);
            break;
        case 'U':
            result = num * exchangeRate;
            printf("Kết quả: %.2f USD = %.2f VND\n", num, result);
            break;
        default:
            printf("Error: Lựa chọn không hợp lệ.\n");
    }

    return 0;
}