# Step-by-Step Guide: Building a Web Crawler in C with PostgreSQL and Inverted Index

This guide will help you build a simple web crawler in C, store crawled data in PostgreSQL, and implement an inverted index for search functionality.

## What is a Web Crawler?
A web crawler is a program that automatically visits web pages, downloads their content, and follows links to discover more pages. Search engines use crawlers to index the web.

## What is an Inverted Index?
An inverted index is a data structure that maps words (terms) to the documents (web pages) where they appear. This makes searching for documents containing a word very fast.

---

## Prerequisites
- **Basic C programming:** If you are new to C, check out [Learn-C.org](https://www.learn-c.org/).
- **PostgreSQL:** A popular open-source database. [Install guide](https://www.postgresql.org/download/)
- **Libraries:**
  - `libpq` (PostgreSQL C client library)
  - `libcurl` (for HTTP requests)
  - `libxml2` (for HTML parsing)
- **Compiler:** GCC or another C compiler

---

## Step 1: Set Up Your Environment
1. **Install dependencies:**
   ```bash
   sudo apt update
   sudo apt install libpq-dev libcurl4-openssl-dev libxml2-dev gcc make
   ```
2. **Create your project directory:**
   ```bash
   mkdir -p ~/Projects/SearchEngine/WebCrawler
   cd ~/Projects/SearchEngine/WebCrawler
   ```

---

## Step 2: Design the Database Schema
1. **Create a PostgreSQL database:**
   ```bash
   createdb search_engine
   ```
2. **Create tables for documents and inverted index:**
   Open the PostgreSQL prompt with `psql search_engine` and run:
   ```sql
   CREATE TABLE documents (
     id SERIAL PRIMARY KEY,
     url TEXT UNIQUE NOT NULL,
     content TEXT
   );

   CREATE TABLE inverted_index (
     term TEXT NOT NULL,
     document_id INTEGER REFERENCES documents(id),
     positions INTEGER[],
     PRIMARY KEY (term, document_id)
   );
   ```

---

## Step 3: Fetch Web Pages in C
- Use `libcurl` to download web pages.
- **Example:** Fetch a URL and print the HTML content. (See [libcurl easy example](https://curl.se/libcurl/c/example.html))
- You will write a C function that takes a URL and returns the HTML as a string.

---

## Step 4: Parse HTML and Extract Links/Text
- Use `libxml2` to parse HTML.
- Extract:
  - **Links:** So your crawler can discover more pages.
  - **Text:** So you can build the inverted index.
- **Tip:** Start by extracting just the visible text (ignore scripts/styles).

---

## Step 5: Store Documents in PostgreSQL
- Use `libpq` to connect to PostgreSQL from C.
- Insert each crawled page into the `documents` table.
- **Tip:** Store the URL and the HTML/text content.

---

## Step 6: Build the Inverted Index
- For each word in the document:
  - Store which document it appears in and its positions (word offsets).
- Use SQL `INSERT` statements from C via `libpq`.
- **Why?** This lets you quickly find all documents containing a word.

---

## Step 7: Implement the Crawler Loop
- **Queue:** Keep a list of URLs to visit (a queue).
- For each URL:
  1. Fetch the page (Step 3).
  2. Parse and extract links (Step 4) — add new URLs to the queue.
  3. Parse and extract text (Step 4).
  4. Store the document and update the inverted index (Steps 5 & 6).
  5. Mark the URL as crawled (so you don't visit it again).
- **Tip:** Start with a single URL (seed), crawl a few pages, and print what you find.

---

## Step 8: Search Functionality
- To search, query the `inverted_index` for documents containing the search term(s).
- Retrieve and rank documents based on how often the term appears.
- **Example:**
   ```sql
   SELECT d.url, COUNT(*) as freq
   FROM inverted_index i
   JOIN documents d ON i.document_id = d.id
   WHERE i.term = 'example'
   GROUP BY d.url
   ORDER BY freq DESC;
   ```

---

## Step 9: Compile and Run
- Write a `Makefile` to build your project:
   ```makefile
   CC=gcc
   CFLAGS=-Wall -g $(shell pkg-config --cflags libpq libcurl libxml-2.0)
   LDFLAGS=$(shell pkg-config --libs libpq libcurl libxml-2.0)
   
   all: crawler
   
   crawler: crawler.o
   	$(CC) -o crawler crawler.o $(LDFLAGS)
   
   clean:
   	rm -f crawler *.o
   ```
- Build:
   ```bash
   make
   ```
- Run:
   ```bash
   ./crawler
   ```

---

## Step 10: Next Steps
- Add politeness (delay between requests).
- Handle robots.txt (so you don't crawl forbidden pages).
- Improve HTML parsing and error handling.
- Add multi-threading for faster crawling.
- Make your search smarter (rank by relevance, support multiple terms, etc).

---

# Step-by-Step: Implementing and Testing Each Function

Below are steps for each function in your codebase, with a suggestion for how to test each one.

## env.h
### 1. env_load
- **Purpose:** Load environment variables from a file (e.g., `.env`).
- **What should it do?**
  - Read key-value pairs from a file (e.g., `KEY=value` lines).
  - Store them in memory for fast lookup.
  - Ignore comments and blank lines.
- **Why?**
  - Allows you to configure your app (database credentials, etc.) without hardcoding values.
- **How to test:**
  - Create a `.env` file:
    ```env
    TEST_KEY=hello
    TEST_NUM=42
    # This is a comment
    
    EMPTY=
    ```
  - Example test code:
    ```c
    env_load(".env");
    printf("TEST_KEY = %s\n", env_get("TEST_KEY", NULL)); // hello
    printf("TEST_NUM = %s\n", env_get("TEST_NUM", NULL)); // 42
    printf("EMPTY = %s\n", env_get("EMPTY", "default")); // (empty string)
    env_free();
    ```

### 2. env_get
- **Purpose:** Get the value of an environment variable.
- **What should it do?**
  - Return the value for a given key, or the default if not found.
- **Why?**
  - Lets you access configuration anywhere in your code.
- **How to test:**
  - After loading, call `env_get("POSTGRES_USER", "none")` and print the result.
    ```c
    env_load(".env");
    printf("POSTGRES_USER = %s\n", env_get("POSTGRES_USER", "none"));
    env_free();
    ```

### 3. env_set
- **Purpose:** Set or override an environment variable.
- **What should it do?**
  - Add or update a key-value pair in memory.
- **Why?**
  - Useful for tests or dynamic configuration.
- **How to test:**
  - Example:
    ```c
    env_set("MY_VAR", "123");
    printf("MY_VAR = %s\n", env_get("MY_VAR", NULL)); // 123
    env_free();
    ```

### 4. env_free
- **Purpose:** Free all loaded environment variables.
- **What should it do?**
  - Release all memory used for environment variables.
- **Why?**
  - Prevents memory leaks.
- **How to test:**
  - Use `valgrind` or `asan` to check for memory leaks after calling `env_free()`.
    ```c
    env_load(".env");
    env_free();
    // Run with: valgrind ./your_test_binary
    ```

## db_manager.h
### 5. db_manager_init
- **Purpose:** Initialize the database connection.
- **What should it do?**
  - Connect to PostgreSQL using credentials from environment variables.
  - Return 0 on success, nonzero on failure.
- **Why?**
  - Needed before any database operation.
- **How to test:**
  - Example:
    ```c
    if (db_manager_init() == 0) {
        printf("DB connected!\n");
        db_manager_close();
    } else {
        printf("DB connection failed!\n");
    }
    ```
  - Try with correct and incorrect credentials.

### 6. db_manager_close
- **Purpose:** Close the database connection.
- **What should it do?**
  - Cleanly disconnect from PostgreSQL.
- **Why?**
  - Frees resources and avoids connection leaks.
- **How to test:**
  - Call after `db_manager_init()`. Check for errors or crashes.
    ```c
    db_manager_init();
    db_manager_close();
    ```

### 7. db_manager_insert_document
- **Purpose:** Insert a crawled document into the database.
- **What should it do?**
  - Insert a new row into the `documents` table with the given URL and content.
  - Return the new document ID, or -1 on error.
- **Why?**
  - Stores crawled pages for indexing and search.
- **How to test:**
  - Example:
    ```c
    db_manager_init();
    int doc_id = db_manager_insert_document("http://example.com", "<html>...</html>");
    printf("Inserted doc_id: %d\n", doc_id);
    db_manager_close();
    ```
  - Check in PostgreSQL:
    ```sql
    SELECT * FROM documents WHERE id = <doc_id>;
    ```

### 8. db_manager_insert_term
- **Purpose:** Insert a term into the inverted index for a document.
- **What should it do?**
  - Insert a row into `inverted_index` with the term, document ID, and positions array.
- **Why?**
  - Enables fast search for terms in documents.
- **How to test:**
  - Example:
    ```c
    int positions[] = {1, 5, 10};
    db_manager_init();
    int doc_id = db_manager_insert_document("http://example.com", "test content");
    db_manager_insert_term("test", doc_id, positions, 3);
    db_manager_close();
    ```
  - Check in PostgreSQL:
    ```sql
    SELECT * FROM inverted_index WHERE term = 'test';
    ```

## html_parser.h
### 9. html_parser_extract_links
- **Purpose:** Extract all links from HTML.
- **What should it do?**
  - Parse the HTML and return all `href` values from `<a>` tags as absolute URLs.
  - Use the base URL to resolve relative links.
- **Why?**
  - Finds new pages to crawl.
- **How to test:**
  - Example:
    ```c
    const char *html = "<a href='http://a.com'>A</a> <a href='b.html'>B</a> <a href='/c'>C</a>";
    char **links = html_parser_extract_links(html, "http://example.com");
    for (int i = 0; links[i]; ++i) printf("Link: %s\n", links[i]);
    utils_free_string_array(links);
    // Output should be:
    // Link: http://a.com
    // Link: http://example.com/b.html
    // Link: http://example.com/c
    ```

### 10. html_parser_extract_text
- **Purpose:** Extract visible text from HTML.
- **What should it do?**
  - Remove all HTML tags and return only the readable text.
  - Ignore scripts, styles, and comments.
- **Why?**
  - Used for indexing and searching page content.
- **How to test:**
  - Example:
    ```c
    const char *html = "<html><body>Hello <b>world</b>!<script>ignore()</script></body></html>";
    char *text = html_parser_extract_text(html);
    printf("Text: %s\n", text); // Hello world!
    free(text);
    ```

## http_client.h
### 11. http_client_fetch
- **Purpose:** Fetch the content of a URL.
- **What should it do?**
  - Perform an HTTP GET request to the given URL.
  - Return the response body and set the HTTP status code.
- **Why?**
  - Downloads web pages for crawling.
- **How to test:**
  - Example:
    ```c
    long code;
    char *body = http_client_fetch("http://example.com", &code);
    printf("HTTP code: %ld\nBody: %s\n", code, body);
    free(body);
    // Try with valid and invalid URLs.
    ```

## crawler.h
### 12. crawler_start
- **Purpose:** Start the crawling process from a seed URL.
- **What should it do?**
  - Begin crawling from the given URL, following links and storing pages.
  - Should use the other modules (HTTP, HTML parser, DB, etc.).
- **Why?**
  - Main entry point for your crawler.
- **How to test:**
  - Example:
    ```c
    crawler_start("http://example.com");
    // Observe logs/output for crawling progress.
    crawler_stop();
    // Check DB for new documents.
    ```

### 13. crawler_stop
- **Purpose:** Stop the crawler and clean up resources.
- **What should it do?**
  - Gracefully stop crawling and free all resources.
- **Why?**
  - Prevents memory leaks and allows clean shutdown.
- **How to test:**
  - Call after `crawler_start()`. Check for clean shutdown and no memory leaks (use valgrind).

## url_utils.h
### 14. url_utils_normalize
- **Purpose:** Normalize a URL. This means converting a possibly relative or non-canonical URL into a standard, absolute form. This helps avoid crawling the same page multiple times under different URLs.
- **What should it do?**
  - Resolve relative URLs (e.g., `/about` with base `http://example.com` → `http://example.com/about`)
  - Remove fragments (e.g., `http://example.com/page#section` → `http://example.com/page`)
  - Remove duplicate slashes (e.g., `http://example.com//about` → `http://example.com/about`)
  - Optionally, lowercase the scheme and host (e.g., `HTTP://EXAMPLE.COM/` → `http://example.com/`)
  - Remove default ports (e.g., `http://example.com:80/` → `http://example.com/`)
- **Why?**
  - Ensures each page is crawled only once, even if linked with different forms.
- **How to test:**
  - Example:
    ```c
    // Relative URL
    char *norm1 = url_utils_normalize("/about", "http://example.com");
    printf("Normalized: %s\n", norm1); // http://example.com/about
    free(norm1);

    // With fragment
    char *norm2 = url_utils_normalize("http://example.com/page#section", NULL);
    printf("Normalized: %s\n", norm2); // http://example.com/page
    free(norm2);

    // Duplicate slashes
    char *norm3 = url_utils_normalize("http://example.com//about", NULL);
    printf("Normalized: %s\n", norm3); // http://example.com/about
    free(norm3);

    // Uppercase scheme/host
    char *norm4 = url_utils_normalize("HTTP://EXAMPLE.COM/", NULL);
    printf("Normalized: %s\n", norm4); // http://example.com/
    free(norm4);

    // Default port
    char *norm5 = url_utils_normalize("http://example.com:80/", NULL);
    printf("Normalized: %s\n", norm5); // http://example.com/
    free(norm5);
    ```

### 15. url_utils_is_valid
- **Purpose:** Check if a URL is valid.
- **What should it do?**
  - Return 1 if the URL is a valid HTTP or HTTPS URL, 0 otherwise.
  - Should check for a valid scheme, host, and optionally path.
- **Why?**
  - Prevents the crawler from following invalid or unsupported URLs.
- **How to test:**
  - Example:
    ```c
    printf("Valid: %d\n", url_utils_is_valid("http://example.com")); // 1
    printf("Invalid: %d\n", url_utils_is_valid("ftp://example.com")); // 0
    printf("Invalid: %d\n", url_utils_is_valid("not a url")); // 0
    ```

## frontier.h
### 16. frontier_init
- **Purpose:** Initialize the URL frontier.
- **What should it do?**
  - Set up the data structure (queue or set) for storing URLs to crawl.
- **Why?**
  - Needed before adding or removing URLs from the frontier.
- **How to test:**
  - Example:
    ```c
    frontier_init();
    frontier_free();
    // Should not crash or leak memory.
    ```

### 17. frontier_enqueue
- **Purpose:** Add a URL to the frontier.
- **What should it do?**
  - Add the given URL to the queue/set for future crawling.
- **Why?**
  - Allows the crawler to discover and schedule new pages.
- **How to test:**
  - Example:
    ```c
    frontier_init();
    frontier_enqueue("http://a.com");
    char *url = frontier_dequeue();
    printf("Dequeued: %s\n", url); // http://a.com
    free(url);
    frontier_free();
    ```

### 18. frontier_dequeue
- **Purpose:** Get the next URL from the frontier.
- **What should it do?**
  - Remove and return the next URL to crawl, or NULL if empty.
- **Why?**
  - Drives the crawling process.
- **How to test:**
  - See above. Enqueue multiple URLs, dequeue and print each.

### 19. frontier_is_empty
- **Purpose:** Check if the frontier is empty.
- **What should it do?**
  - Return 1 if no URLs are left to crawl, 0 otherwise.
- **Why?**
  - Lets the crawler know when to stop.
- **How to test:**
  - Example:
    ```c
    frontier_init();
    printf("Empty: %d\n", frontier_is_empty()); // 1
    frontier_enqueue("http://a.com");
    printf("Empty: %d\n", frontier_is_empty()); // 0
    char *url = frontier_dequeue();
    free(url);
    printf("Empty: %d\n", frontier_is_empty()); // 1
    frontier_free();
    ```

### 20. frontier_free
- **Purpose:** Free all resources used by the frontier.
- **What should it do?**
  - Release all memory and resources used by the frontier data structure.
- **Why?**
  - Prevents memory leaks.
- **How to test:**
  - Always call at the end of your tests. Use valgrind to check for leaks.

## utils.h
### 21. utils_split_words
- **Purpose:** Split a string into words.
- **What should it do?**
  - Break a string into an array of words (tokens), ignoring punctuation.
  - Return a NULL-terminated array of strings.
- **Why?**
  - Used for building the inverted index.
- **How to test:**
  - Example:
    ```c
    char **words = utils_split_words("hello world, this is a test!");
    for (int i = 0; words[i]; ++i) printf("Word: %s\n", words[i]);
    utils_free_string_array(words);
    // Output should be:
    // Word: hello
    // Word: world
    // Word: this
    // Word: is
    // Word: a
    // Word: test
    ```

### 22. utils_free_string_array
- **Purpose:** Free a NULL-terminated array of strings.
- **What should it do?**
  - Free all memory allocated for the array and its strings.
- **Why?**
  - Prevents memory leaks.
- **How to test:**
  - See above. Use valgrind to check for leaks.

## robots_parser.h
### 23. robots_parser_is_allowed
- **Purpose:** Check if a URL is allowed by robots.txt.
- **What should it do?**
  - Parse the robots.txt rules and determine if the given user agent can crawl the URL.
  - Return 1 if allowed, 0 if disallowed.
- **Why?**
  - Respects website crawling policies.
- **How to test:**
  - Example:
    ```c
    const char *robots = "User-agent: *\nDisallow: /private";
    printf("Allowed: %d\n", robots_parser_is_allowed(robots, "MyBot", "/public/page")); // 1
    printf("Disallowed: %d\n", robots_parser_is_allowed(robots, "MyBot", "/private/data")); // 0
    ```

## logger.h
### 24. log_internal
- **Purpose:** Log a message with a given level, file, and line.
- **What should it do?**
  - Print a formatted log message with timestamp, log level, file, and line number.
- **Why?**
  - Helps debug and monitor your crawler.
- **How to test:**
  - Example:
    ```c
    log_internal(INFO, __FILE__, __LINE__, "This is an info message: %d", 42);
    log_internal(ERROR, __FILE__, __LINE__, "This is an error!");
    // Output should include timestamp, level, file, and line.
    ```

### 25. log_debug, log_info, log_warning, log_error, log_msg
- **Purpose:** Log messages at different levels using macros.
- **What should it do?**
  - Provide convenient wrappers for logging at different levels.
- **Why?**
  - Makes logging easier and more readable.
- **How to test:**
  - Example:
    ```c
    log_debug("Debug message");
    log_info("Info message");
    log_warning("Warning message");
    log_error("Error message");
    log_msg(INFO, "Custom info: %s", "hello");
    // Output should show the correct log level and message.
    ```

---

# Tips for Beginners:
- Start small! Crawl just one or two pages and print the results.
- Build each part separately (fetch, parse, store, index, search).
- Use lots of print statements to debug and understand what your code is doing.
- Read documentation for each library (`libcurl`, `libxml2`, `libpq`).
- Ask for help or search online if you get stuck.

**You can do it!**
