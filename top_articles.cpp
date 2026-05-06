#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

/* curl needs a callback to collect the HTTP response into a string —
   it writes chunks of data as they arrive, we just keep appending */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/* fetch a single page from the API and return it as parsed JSON */
json fetchPage(int page) {
    CURL* curl = curl_easy_init();
    string response;

    string url = "https://jsonmock.hackerrank.com/api/articles?page=" + to_string(page);

    /* configure curl: set URL, tell it where to write data, then fire the request */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return json::parse(response);
}

vector<string> topArticles(int limit) {

    /* Step 1: fetch page 1 first — we need total_pages before looping */
    json first = fetchPage(1);
    int total_pages = first["total_pages"];

    /* Step 2: store each valid article as (num_comments, name) pair */
    vector<pair<int, string>> articles;

    /* lambda to process any page's data array and push valid articles */
    auto processPage = [&](json& data) {
        for (auto& item : data["data"]) {

            /* article name rule: title > story_title > skip entirely */
            string name = "";
            if (!item["title"].is_null())
                name = item["title"];
            else if (!item["story_title"].is_null())
                name = item["story_title"];
            else
                continue; // both null — skip this article

            /* treat null num_comments as 0 per the spec */
            int comments = item["num_comments"].is_null() ? 0 : item["num_comments"].get<int>();

            articles.push_back({comments, name});
        }
    };

    /* process first page already fetched, then loop through the rest */
    processPage(first);
    for (int p = 2; p <= total_pages; p++) {
        json page = fetchPage(p);
        processPage(page);
    }

    /* Step 3: sort — more comments = higher rank; tie = alphabetically descending */
    sort(articles.begin(), articles.end(), [](const pair<int,string>& a, const pair<int,string>& b) {
        if (a.first != b.first) return a.first > b.first; // descending by comments
        return a.second > b.second;                        // descending alphabetically
    });

    /* Step 4: extract just the names up to the limit */
    vector<string> result;
    for (int i = 0; i < limit && i < (int)articles.size(); i++) {
        result.push_back(articles[i].second);
    }

    return result;
}

int main() {
    int limit;
    cout << "Enter limit: ";
    cin >> limit;

    vector<string> top = topArticles(limit);

    for (const string& name : top) {
        cout << name << "\n";
    }

    return 0;
}