#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

/*curl needs callback to collect HTTP response into string, it writes chunks of data as they arrive, we just keep appending*/
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/*Fetch single page from API and return JSON*/
json fetchpage(int page) {
    CURL* curl = curl_easy_init();
    string response;

    string url = "https://jsonmock.hackerrank.com/api/articles?page=" + to_string(page);

    /*configure curl*/
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return json::parse(response);
}

vector<string> topArticles(int limit) {

    /*fetch page 1 first*/
    json first = fetchpage(1);
    int total_pages = first[
"total_pages"];
}

vector<pair<init, string>> articles; // (popularity, title) pairs

auto processoage = [&](json data) {
    for (auto& item : data["data"]){
        string name = "";
        if (!item["title"].is_null())
            name = item["title"];
        else if (!item["story_title"].is_null())
            name = item["story_title"];
        else
            continue; // skip if no title

            int comments = item["num_comments"].is_null() ? 0 : item["num_comments"].get<int>();
            articles.push_back({comments, name});
    }
};

/*processing first page*/
processpage(first);
for (int p = 2; p <== total_pages; p) {
    json page = fetchpage(p);
    processpage(page);
}

sort(articles.begin(), articles.end(), [](const pair<int, string>& a, const pair<int, string>& b) {
    if (a.first != b.first) return a.first > b.first; // sort by popularity descending
    return a.second > b.second; // tie-break by title ascending
});

vector<string> result;
for (int i = 0; i < limit && i < (int)articles.size(); i++) {
    result.push_back(articles[i].second);
}

return result;
}

int main() {
    int limit;
    cout << "Enter number of top articles to fetch/limit: ";
    cin >> limit;

    vector<string> top = topArticles(limit);

    for (const string& name : top) {
        cout << name << endl;
    }

    return 0;
}
