
# 重要数据结构

typedef struct IrisCode
{
  string name;
  unsigned char **data;
  int stepsize;
} IrisCode;

static const int CODE_WIDTH = 512;
static const int CODE_HEIGHT = 20;
static const int MIN_SHIFT = -16;
static const int MAX_SHIFT = 16;
static const int htlut[256] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
static const int ALG_MINHD = 0, ALG_3A = 1, ALG_3AL = 2, ALG_3AS = 3;
static const int MODE_MAIN = 1, MODE_HELP = 2;

# Usage
![](http://opkk27k9n.bkt.clouddn.com/17-7-5/4014078.jpg)

生成的dynamic.txt如下：
hd(../../../data/iris/#S1002_R_&INX_@000_CASIA_04.png,../../../data/iris/#S1002_R_&INX_@000_CASIA_05.png):0.133984	 time: 159 micro seconds 	 step-size: 4
hd(../../../data/iris/#S1002_R_&INX_@000_CASIA_04.png,../../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.453906	 time: 132 micro seconds 	 step-size: 4
hd(../../../data/iris/#S1002_R_&INX_@000_CASIA_05.png,../../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.459766	 time: 127 micro seconds 	 step-size: 4

切换算法为 Triple-limited algorithm：
$ ./TripleA -f ../../data/iris -a 3al
得到的dynamic.txt增加以下内容（时间更短，速度更快）：
#S1002_R_&INX_@000_CASIA_05.png):0.133984	 time: 115 micro seconds 	 step-size: 4
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_04.png,../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.460156	 time: 103 micro seconds 	 step-size: 4
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_05.png,../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.472266	 time: 106 micro seconds 	 step-size: 4

切换算法为TripleA-single-sided algorithm (3as):
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_04.png,../../data/iris/#S1002_R_&INX_@000_CASIA_05.png):0.133984	 time: 45 micro seconds 	 step-size: 4
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_04.png,../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.475781	 time: 69 micro seconds 	 step-size: 4
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_05.png,../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.488672	 time: 64 micro seconds 	 step-size: 4


设定step-size为5，得到static.txt:
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_04.png,../../data/iris/#S1002_R_&INX_@000_CASIA_05.png):0.133984	 time: 39 micro seconds 	 step-size: 5
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_04.png,../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.475781	 time: 37 micro seconds 	 step-size: 5
hd(../../data/iris/#S1002_R_&INX_@000_CASIA_05.png,../../data/iris/#S1002_R_&INX_@000_CASIA_03.png):0.488672	 time: 37 micro seconds 	 step-size: 5

# 函数
// estimate the Hamming distance between two given codes using the TripleA algorithm
void TripleA(IrisCode code1, IrisCode code2, int start, int stop, ofstream& log, int stepsize);
// estimate the Hamming distance between two given codes using the TripleA-limited algorithm
void TripleA_limited(IrisCode code1, IrisCode code2, int start, int stop, ofstream& log, int stepsize);
// estimate the Hamming distance between two given codes using the TripleA-single-sided algorithm
void TripleA_singlesided(IrisCode code1, IrisCode code2, int start, int stop, ofstream& log, int stepsize);
// read an iris-code form a given file name, extract the (dynamic) step size
// and map it to a more efficient representation
IrisCode getCode(string filename, float k);

# main



# commandline functions
/** ------------------------------- commandline functions ------------------------------- **/

/**
 * Parses a command line
 * This routine should be called for parsing command lines for executables.
 * Note, that all options require '-' as prefix and may contain an arbitrary
 * number of optional arguments.
 *
 * cmd: commandline representation
 * argc: number of parameters
 * argv: string array of argument values
 */


## Timing functions
class Timing...

C++库：boost::posix_time


## file pattern matching functions
boost

/** ------------------------------- file pattern matching functions ------------------------------- **/


/*
 * Formats a given string, such that it can be used as a regular expression
 * I.e. escapes special characters and uses * and ? as wildcards
 *
 * pattern: regular expression path pattern
 * pos: substring starting index
 * n: substring size
 *
 * returning: escaped substring
 */


# 流程图
<iframe id="embed_dom" name="embed_dom" frameborder="0" style="border:1px solid #000;display:block;width:430px; height:320px;" src="https://www.processon.com/embed/582d9618e4b06bc83a130cb1">






