/*add ngtcp */

unsigned int* extract_response_codes_ngtcp(unsigned char* buf, unsigned int buf_size, unsigned int* state_count_ref);

region_t* extract_requests_ngtcp(unsigned char* buf, unsigned int buf_size, unsigned int* region_count_ref);
