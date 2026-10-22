#include <stdio.h>
#include <stdlib.h>
int main() {
    unsigned char data[256] = {
        0x09,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00,
        0xFF
    };
    unsigned char ptr = 0;
    srand((long long)&ptr);
    ptr = data[0];
    while(data[ptr] != 0xFF) {
        unsigned char cur = data[ptr];
        switch(cur & 0xF0) {
            case 0x00: { // system services
                switch(cur & 0x0F) {
                    case 0x00: { // no op
						ptr += 4;
                        break;
                    }
                    case 0x01: { // halt
                        return data[ptr + 1];
                    }
					case 0x02: { // hang
						break;
					}
                    case 0x03: { // reset
                        ptr = data[0];
                        break;
                    }
                    case 0x04: { // jump
                        ptr = data[ptr + 1];
                        break;
                    }
                    case 0x05: { // eq jump
                        if(data[data[ptr + 2]] == data[data[ptr + 3]]) ptr = data[ptr + 1];
                        else ptr += 4;
                        break;
                    }
                    case 0x06: { // neq jump
                        if(data[data[ptr + 2]] != data[data[ptr + 3]]) ptr = data[ptr + 1];
                        else ptr += 4;
                        break;
                    }
                    case 0x07: { // data jump
                        ptr = data[data[ptr + 1]];
                        break;
                    }
                    case 0x08: { // data eq jump
                        if(data[data[ptr + 2]] == data[data[ptr + 3]]) ptr = data[data[ptr + 1]];
                        else ptr += 4;
                        break;
                    }
                    case 0x09: { // data neq jump
                        if(data[data[ptr + 2]] != data[data[ptr + 3]]) ptr = data[data[ptr + 1]];
                        else ptr += 4;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                break;
            }
            case 0x10: { // math services
                switch(cur & 0x0F) {
                    case 0x00: {
                        data[data[ptr + 3]] = data[data[ptr + 1]];
                        break;
                    }
                    case 0x01: { // these should be obvious: add, sub, mult, div
                        data[data[ptr + 3]] = data[data[ptr + 1]] + data[data[ptr + 2]];
                        break;
                    }
                    case 0x02: {
                        data[data[ptr + 3]] = data[data[ptr + 1]] - data[data[ptr + 2]];
                        break;
                    }
                    case 0x03: {
                        data[data[ptr + 3]] = data[data[ptr + 1]] * data[data[ptr + 2]];
                        break;
                    }
                    case 0x04: {
                        data[data[ptr + 3]] = data[data[ptr + 1]] / data[data[ptr + 2]];
                        break;
                    }
                    case 0x05: {
                        data[data[ptr + 3]] = data[data[ptr + 1]] % data[data[ptr + 2]];
                        break;
                    }
                    case 0x06: { // raw rand
                        data[data[ptr + 3]] = rand() % 256;
                        break;
                    }
					//case 0x07: { // range rand
                    //    data[data[ptr + 3]] = data[data[ptr + 2]] + (rand() % (data[data[ptr + 1]] - data[data[ptr + 2]] + 1));
                    //    break;
                    //}
                    default: {
                        break;
                    }
                }
                break;
            }
            case 0x20: { // user services
                switch(cur & 0x0F) {
                    case 0x01: { // hex I/O
                        if(data[ptr + 1] == 1) {
                            fprintf(stdout, "%x", data[data[ptr + 2]]);
                            if(!data[ptr + 3]) printf("\n");
                        }
                        if(data[ptr + 1] == 2) {
                            fprintf(stderr, "%x", data[data[ptr + 2]]);
                            if(!data[ptr + 3]) printf("\n");
                        }
                        if(data[ptr + 1] == 0) {
                            scanf("%x", &data[data[ptr + 2]]);
                        }
                        break;
                    }
                    case 0x02: { // int I/O
                        if(data[ptr + 1] == 1) {
                            fprintf(stdout, "%d", data[data[ptr + 2]]);
                            if(!data[ptr + 3]) printf("\n");
                        }
                        if(data[ptr + 1] == 2) {
                            fprintf(stderr, "%d", data[data[ptr + 2]]);
                            if(!data[ptr + 3]) printf("\n");
                        }
                        if(data[ptr + 1] == 0) {
                            scanf("%d", &data[data[ptr + 2]]);
                        }
                        break;
                    }
                    case 0x03: { // char I/O
                        if(data[ptr + 1] == 1) {
                            fprintf(stdout, "%c", data[data[ptr + 2]]);
                            if(!data[ptr + 3]) printf("\n");
                        }
                        if(data[ptr + 1] == 2) {
                            fprintf(stderr, "%c", data[data[ptr + 2]]);
                            if(!data[ptr + 3]) printf("\n");
                        }
                        if(data[ptr + 1] == 0) {
                            scanf("%c", &data[data[ptr + 2]]);
                        }
                        break;
                    }
                    case 0x05: {
                        fputs("\a", stdout); // beep
                        break;
                    }
                    default: {
                        break;
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
        if((cur & 0xF0) != 0x00) ptr += 4;
        //printf("%d\n", ptr);
    }
    return 0;
}