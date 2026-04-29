#ifndef CRAWLER_H
#define CRAWLER_H

#ifdef __cplusplus
extern "C" {
#endif

// Start the crawling process from a seed URL
void crawler_start(const char *seed_url);

// Stop the crawler and clean up resources
void crawler_stop();

#ifdef __cplusplus
}
#endif

#endif // CRAWLER_H
