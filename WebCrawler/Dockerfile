# Use an official GCC image as the build environment
FROM gcc:13 as builder

# Install dependencies
RUN apt-get update && \
    apt-get install -y libpq-dev libcurl4-openssl-dev libxml2-dev make && \
    rm -rf /var/lib/apt/lists/*

# Set work directory
WORKDIR /app

# Copy source code
COPY . /app

# Build the crawler
RUN make

# Use a minimal runtime image
FROM debian:bookworm-slim

# Install runtime dependencies
RUN apt-get update && \
    apt-get install -y libpq5 libcurl4 libxml2 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the built binary and any needed files
COPY --from=builder /app/crawler /app/
COPY --from=builder /app/.env /app/.env

CMD ["./crawler"]
