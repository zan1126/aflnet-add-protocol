region_t* extract_requests_ngtcp(unsigned char* buf, unsigned int buf_size, unsigned int* region_count_ref)
{
  char *mem;
  unsigned int byte_count = 0;
  unsigned int mem_count = 0;
  unsigned int mem_size = 1024;
  unsigned int region_count = 0;
  region_t *regions = NULL;
  
  mem = (char *)ck_alloc(mem_size);
  
  unsigned int cur_start = 0;
  unsigned int cur_end = 0;
  
  while (byte_count < buf_size) {
	  memcpy(&mem[mem_count], buf + byte_count++, 1);
	  
	  //When the highest bit in the first byte in the header is 1,it is the long header
	  if(mem[mem_count] & 128 == 128) {
		  //When the fifth and sixth bytes are both 0,it is the Initial packet
		  if((~mem[mem_count] & 16 == 16) && (~mem[mem_count] &32 == 32)) { 
			  
			  //The header length of the initial packet is 45
			  if(mem_count >= 45) {
				  u16* size_buf = (u16*)&mem[43];
				  u16 message_size = (u16)ntohs(*size_buf);
				  
				  unsigned int bytes_to_skip = message_size;
				  
				  unsigned int temp_count = 0;
				  while ((byte_count < buf_size) && (temp_count < bytes_to_skip)) {
					  byte_count++;
					  cur_end++;
					  temp_count++;
				  }
				  
				  if(byte_count < buf_size) {
					  byte_count--;
					  cur_end--;
				  }
				  
				  region_count++;
				  regions = (region_t *)ck_realloc(regions, region_count * sizeof(region_t));
				  regions[region_count -1].start_byte = cur_start;
				  regions[region_count -1].end_byte = cur_end;
				  regions[region_count -1].state_sequence = NULL;
				  regions[region_count -1].state_count = 0;
				  
				  if(cur_end < buf_size -1) {
					  mem_count = 0;
					  cur_start = cur_end +1;
					  cur_end = cur_start;
				  }
			  }
		  } 
		  ////When the fifth byte is 0 and the sixth byte is 1,it is the Handshake packet
		  else if((~mem[mem_count] & 16 == 16) && (mem[mem_count] & 32 == 32)) { 
			  //The header length of the handshake packet is 44
			  if(mem_count >= 44) {
				  u16* size_buf = (u16*)&mem[42];
				  u16 message_size = (u16)ntohs(*size_buf);
				  
				  unsigned int bytes_to_skip = message_size;
				  
				  unsigned int temp_count = 0;
				  while ((byte_count < buf_size) && (temp_count < bytes_to_skip)) {
					  byte_count++;
					  cur_end++;
					  temp_count++;
				  }
				  
				  if(byte_count < buf_size) {
					  byte_count--;
					  cur_end--;
				  }
				  
				  region_count++;
				  regions = (region_t *)ck_realloc(regions, region_count * sizeof(region_t));
				  regions[region_count -1].start_byte = cur_start;
				  regions[region_count -1].end_byte = cur_end;
				  regions[region_count -1].state_sequence = NULL;
				  regions[region_count -1].state_count = 0;
				  
				  if(cur_end < buf_size -1) {
					  mem_count = 0;
					  cur_start = cur_end +1;
					  cur_end = cur_start;
				  }
			  }
		  }
	  }
	  //When the highest bit in the first byte in the header is 0,it is the short header
	  else if(~mem[mem_count] & 128 == 128) {
		  //the length of the header is 19
		  if(mem_count >= 19) {
			  cur_end++;
			  byte_count++;
			  region_count++;
			  regions = (region_t *)ck_realloc(regions, region_count * sizeof(region_t));
			  regions[region_count -1].start_byte = cur_start;
			  regions[region_count -1].end_byte = cur_end;
			  regions[region_count -1].state_sequence = NULL;
			  regions[region_count -1].state_count = 0;
			  
			  if(cur_end == buf_size -1) break;
			  
			  mem_count = 0;
			  cur_start = cur_end +1;
			  cur_end = cur_start;
		  }
	  }else {
			  mem_count ++;
			  cur_end ++;
			  
			  if(cur_end == buf_size - 1) {
				  region_count++;
				  regions = (region_t *)ck_realloc(regions, region_count * sizeof(region_t));
				  regions[region_count - 1].start_byte = cur_start;
				  regions[region_count - 1].end_byte = cur_end;
				  regions[region_count - 1].state_sequence = NULL;
				  regions[region_count - 1].state_count = 0;
				  break;
			  }
			  
			  if(mem_count == mem_size) {
				  mem_size *= 2;
				  mem = (char *)ck_realloc(mem, mem_size);
			  }
		  }
	  
  }
  if(mem) ck_free(mem);
  
  if ((region_count == 0) && (buf_size > 0)) {
	  regions = (region_t *)ck_realloc(regions, sizeof(region_t));
	  regions[0].start_byte = 0;
	  regions[0].end_byte = buf_size - 1;
	  regions[0].state_sequence = NULL;
	  regions[0].state_count = 0;
  
	  region_count = 1;
  }
  
  *region_count_ref = region_count;
  return regions;
 }
