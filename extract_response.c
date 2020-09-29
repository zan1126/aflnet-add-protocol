unsigned int* extract_response_codes_ngtcp(unsigned char* buf, unsigned int buf_size, unsigned int* state_count_ref)
{
	char *mem;
	unsigned int byte_count = 0;
	unsigned int mem_count = 0;
	unsigned int mem_size = 1024;
	unsigned int *state_sequence = NULL;
	unsigned int state_count = 0;
	
	mem=(char *)ck_alloc(mem_size);
	
	state_count++;
	state_sequence = (unsigned int *)ck_realloc(state_sequence, state_count * sizeof(unsigned int));
	state_sequence[state_count - 1] = 0;
	while (byte_count < buf_size) {
		
		memcpy(&mem[mem_count], buf + byte_count++, 1);
		
		if(mem[mem_count] & 128 == 128) {
			if((~mem[mem_count] & 16 == 16) && (~mem[mem_count] & 32 == 32) {
				if(mem_count >= 45) {
					u16* size_buf = (u16*)&mem[43];
					u16 message_size = (u16)ntohs(*size_buf);
					
					unsigned int bytes_to_skip = message_size;
					
					unsigned int temp_count = 0;
					while ((byte_count < buf_size) && (temp_count < bytes_to_skip)) {
						byte_count++;
						temp_count++;
					}
					
					if(byte_count < buf_size) {
						byte_count--;
						cur_end--;
					}
					//According to my understanding, the data packets of this protocol are distinguished according to the first byte, 
					//so this byte is regarded as message_code
		            unsigned char message_code = (unsigned char)mem[0];
		            state_count++;
		            state_sequence = (unsigned int *)ck_realloc(state_sequence, state_count * sizeof(unsigned int));
		            state_sequence[state_count - 1] = message_code;
		            mem_count = 0;
				} else if((~mem[mem_count] & 16 == 16) && (mem[mem_count] & 32 == 32)) {
					if(mem_count >= 44) {
						u16* size_buf = (u16*)&mem[42];
						u16 message_size = (u16)ntohs(*size_buf);
						
						unsigned int bytes_to_skip = message_size;
						
						unsigned int temp_count = 0;
						while ((byte_count < buf_size) && (temp_count < bytes_to_skip)) {
							byte_count++;
							temp_count++;
						}
						
						if(byte_count < buf_size) {
							byte_count--;
							cur_end--;
						}
						unsigned char message_code = (unsigned char)mem[0];
						state_count++;
						state_sequence = (unsigned int *)ck_realloc(state_sequence, state_count * sizeof(unsigned int));
						state_sequence[state_count - 1] = message_code;
						mem_count = 0;
					}
				}
			}
		}else if(~mem[mem_count] & 128 == 128) {
				if(mem_count >= 19) {
					cur_end++;
					byte_count++;
					unsigned char message_code = (unsigned char)mem[0];
					state_count++;
					state_sequence = (unsigned int *)ck_realloc(state_sequence, state_count * sizeof(unsigned int));
					state_sequence[state_count - 1] = message_code;
					mem_count = 0;
				}
		} else {
			mem_count++;
			
			if(mem_count == mem_size) {
				mem_size = mem_size * 2;
				mem=(char *)ck_realloc(mem, mem_size);
			}
		}
	}
	if (mem) ck_free(mem);
	
	*state_count_ref = state_count;
	return state_sequence;
}
