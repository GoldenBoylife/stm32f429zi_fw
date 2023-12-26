#include "cli.h"
#include "uart.h"

#define CLI_CMD_LIST_MAX  8

#ifdef _USE_CLI
typedef struct
{
    char cmd_str[8]; //명령어는 7글자까지
    void (*cmd_func)(uint8_t argc, const char **argv);
    //함수 포인터: 가리키고자 하는 함수 원형 적고
    //함수를 괄호로 감싼다. 맨 앞에 별 넣음.

} cli_cmd_t;



const uint8_t cli_ch = _DEF_UART_CH3;

static char cli_buf[128];  //버퍼 준비
static uint16_t cli_buf_index =0;

static uint16_t cli_argc = 0;
static char *cli_argv[8];   //포인터 배열로 명령어 저장


static uint8_t cli_cmd_count =0;
//static하면 파일 내부에서만 쓸 수 있고 외부에서는 못씀
//명령어 추가할때마다 한개씩 추가됨

static cli_cmd_t cli_cmd_func[CLI_CMD_LIST_MAX];


static void cliHelp(uint8_t argc, const char **argv);
//이중 포인터

bool cliInit(void)
{
    for( int i =0 ; i < CLI_CMD_LIST_MAX; i++)
    {
        cli_cmd_func[i].cmd_func = NULL;
    }
    cliAdd("help", cliHelp); //명령어 등록
    //cliHelp라는 함수가 따로 있고, 그 함수를 
    cliAdd("ls",cliHelp); //다른 명령어 등록

    return true;
}

void cliHelp(uint8_t argc, const char ** argv)
{
  cliPrintf("-------cliHelp--------\r\n");
  for(int i =0 ; i < cli_cmd_count; i++)
  {
    cliPrintf("%s\r\n",cli_cmd_func[i].cmd_str);
  }
  cliPrintf("-------cliHelp--------\r\n");
}


/*내부적으로 쓰는 함수배열에 추가함*/
//명령어 등록함
bool cliAdd(const char *cmd_str, void (*cmd_func)(uint8_t argc,const char **argv))
{
  if(cli_cmd_count >= CLI_CMD_LIST_MAX)
    //더이상 저장할 공간이 없으니까 false반환
    return false;


  strncpy(cli_cmd_func[cli_cmd_count].cmd_str,cmd_str,8);
  //버퍼 갯수 8
  //안전하게
  cli_cmd_func[cli_cmd_count].cmd_func = cmd_func;
  cli_cmd_count++;
  return true;
}

void cliPrintf(const char *fmt, ...)
{//가변인자 받음
  va_list arg;

  /*? 아래 3줄은 모여다니는 건데,,, 기능까먹음. */
  va_start(arg,fmt);
  uartVPrintf(cli_ch,fmt, arg);
  va_end(arg);
}

/*받은 문자열에서 검색함 */
bool cliIsStr(const char *p_arg, const char *p_str)
{
  if(strcmp(p_arg, p_str) ==0)
    return true;
  else
    return false;
}




int32_t cliGetData(const char *p_arg)
{
  int32_t ret;
  ret = (int32_t)strtoul(p_arg, (char **)NULL,0);
  return ret;
}

bool cliKeepLoop(void)
{
  /*data가 없으면 true로 계속 대기*/
  if(uartAvailable(cli_ch) == 0)
  {
    return true;
  } else {
    return false;
  }
}
/*UART의 수신 데이터를 처리해서 명령어를 실행*/
bool cliMain(void) 
{
  bool ret =false;
    
    if(uartAvailable(cli_ch) >0)
    {
      uint8_t rx_data;

      rx_data = uartRead(cli_ch);
      // uartPrintf(cli_ch,"rx data : 0x%02X (%c)\r\n" ,rx_data, rx_data);
      if (rx_data == '\r')
      {
        /*엔터 후 초기화*/
        cli_buf[cli_buf_index] =0;
        cli_buf_index =0;

        uartPrintf(cli_ch, "\r\n"); 
        //엔터 후 줄바꿈

        /*RUN CMD*/
        char *tok;
        char *str_ptr = cli_buf;

        cli_argc =0;
        while((tok = strtok_r(str_ptr," ", &str_ptr)) != NULL)
        { //&str_ptr : 이중포인터, 
          //tok : " "이후 문자열 시작주소 
          cli_argv[cli_argc] = tok;
          cli_argc++;
          //3개 잘렸으면 cli_argc는 3개
        }

        for (int i=0; i<cli_cmd_count;i++)
        {
          if(strcmp(cli_argv[0], cli_cmd_func[i].cmd_str) ==0)
          { //실행하고자 하는 명령어가 리스트에 존재
            if(cli_cmd_func[i].cmd_func != NULL)
            {
              cli_cmd_func[i].cmd_func(cli_argc -1, (const char **)&cli_argv[1]);
              //함수 넘기기
              ret = true;
              break;
            }
          }
        }

//        /*명령어가 test일때 출력*/
//        if(cli_argc ==2 && strcmp(cli_argv[0],"test")==0)
//        {
////            uartPrintf(cli_ch, "run test\n\r");
////            for(int i =0 ; i < cli_argc;i++)
////            {
////              uartPrintf(cli_ch, "%d : %s\r\n",i, cli_argv[i]);
////            }
//          int32_t test_data;
//          test_data = (int32_t)strtoul(cli_argv[1],(char **)NULL,0);
//          uartPrintf(cli_ch, "test data: %d\r\n", test_data);
//
//          ret = true;
//        }


        uartPrintf(cli_ch, "cli# ");
      /*127개 이하면, 버퍼에 저장*/
      }else if(cli_buf_index < (128-1)){
        cli_buf[cli_buf_index] = rx_data;
        cli_buf_index++;

        uartWrite(cli_ch, &rx_data,1);
        
      }
    }
    return ret;
}

#endif  // USE_CLI