#ifndef TTTCOMMONTYPES_H
#define TTTCOMMONTYPES_H
#include <cstddef>
#include <portaudio.h>
#include <pocketsphinx.h>
#include <signal.h>

class QPushButton;
namespace defaults
{
constexpr short INVALID_CELL = -1;
constexpr size_t DEFAULT_BOARD_SIZE = 3;
constexpr unsigned short DEFAULT_MINIMAX_DEPTH = 3;
constexpr unsigned short GUI_CELL_ROW_SPAN = 1;
constexpr unsigned short GUI_CELL_COLUMN_SPAN = 1;
constexpr char X_COLOR[] = "#455A64";
constexpr char O_COLOR[] = "#FF5722";
constexpr char DEFAULT_COLOR[] = "#FFF";
}
static int global_done = 0;
/**
 * @brief The Cell struct represents a board cell and all the necessary
 * information needed about the cell.
 */
struct Cell
{
    QPushButton *cellBtn = nullptr;
    int row = defaults::INVALID_CELL;
    int col = defaults::INVALID_CELL;
    Cell(QPushButton *cellBtn, int row, int col)
        : cellBtn(cellBtn), row(row), col(col) {}
};

/**
 * @brief The TTTOptions struct defines a tic tac toe game options.
 */
struct TTTOptions
{
    size_t boardSize = defaults::DEFAULT_BOARD_SIZE;
    unsigned short miniMaxDepth = defaults::DEFAULT_MINIMAX_DEPTH;
    bool AIopponent = true;
    bool AIstarts = false;
};

struct SpeechToText
{
    PaStream *stream;
    PaError err;
    ps_decoder_t *decoder;
    ps_config_t *config;
    ps_endpointer_t *ep;
    short *frame;
    size_t frame_size;
    const char *hyp;

    static void catch_sig(int signum)
    {
        (void)signum;
        global_done = 1;
    }

    void configuration(){
        config = ps_config_init(NULL);
        ps_default_search_args(config);
        ps_config_set_str(config,"hmm", "/home/ratty/cmusphinx/share/pocketsphinx/model/custom/custom");
        ps_config_set_str(config,"lm", "/home/ratty/cmusphinx/share/pocketsphinx/model/es-es/es-20k.lm");
        ps_config_set_str(config,"dict", "/home/ratty/cmusphinx/share/pocketsphinx/model/custom/es.dict");
    }

    void init(){
        if ((err = Pa_Initialize()) != paNoError)
            E_FATAL("Failed to initialize PortAudio: %s\n",
                    Pa_GetErrorText(err));
        if ((decoder = ps_init(config)) == NULL)
            E_FATAL("PocketSphinx decoder init failed\n");
        if ((ep = ps_endpointer_init(0, 0.0, PS_VAD_LOOSE, 0, 0)) == NULL)
            E_FATAL("PocketSphinx endpointer init failed\n");
        frame_size = ps_endpointer_frame_size(ep);
        if ((frame = (short int*)malloc(frame_size * sizeof(frame[0]))) == NULL)
            E_FATAL_SYSTEM("Failed to allocate frame");
        if ((err = Pa_OpenDefaultStream(&stream, 1, 0, paInt16,
                                        ps_config_int(config, "samprate"),
                                        frame_size, NULL, NULL)) != paNoError)
            E_FATAL("Failed to open PortAudio stream: %s\n",
                    Pa_GetErrorText(err));
        if ((err = Pa_StartStream(stream)) != paNoError)
            E_FATAL("Failed to start PortAudio stream: %s\n",
                    Pa_GetErrorText(err));
        if (signal(SIGINT, catch_sig) == SIG_ERR)
            E_FATAL_SYSTEM("Failed to set SIGINT handler");
    }
    bool hypIsValid(){
        if (hyp != NULL)
            return true;
        return false;
    }
    void listen(){
        hyp = NULL;
        while (!hypIsValid()) {
            const int16 *speech;
            int prev_in_speech = ps_endpointer_in_speech(ep);
            if ((err = Pa_ReadStream(stream, frame, frame_size)) != paNoError) {
                E_ERROR("Error in PortAudio read: %s\n",
                        Pa_GetErrorText(err));
                break;
            }
            speech = ps_endpointer_process(ep, frame);
            if (speech != NULL) {
                if (!prev_in_speech) {
                    fprintf(stderr, "Speech start at %.2f\n",
                            ps_endpointer_speech_start(ep));
                    fflush(stderr); /* For broken MSYS2 terminal */
                    ps_start_utt(decoder);
                }
                if (ps_process_raw(decoder, speech, frame_size, FALSE, FALSE) < 0)
                    E_FATAL("ps_process_raw() failed\n");
                if (!ps_endpointer_in_speech(ep)) {
                    fprintf(stderr, "Speech end at %.2f\n",
                            ps_endpointer_speech_end(ep));
                    fflush(stderr);
                    ps_end_utt(decoder);
                    if ((hyp = ps_get_hyp(decoder, NULL)) != NULL) {
                        printf("%s\n", hyp);
                        fflush(stdout);
                    }
                }
            }
        }
        if ((err = Pa_StopStream(stream)) != paNoError)
            E_FATAL("Failed to stop PortAudio stream: %s\n",
                    Pa_GetErrorText(err));
        if ((err = Pa_Terminate()) != paNoError)
            E_FATAL("Failed to terminate PortAudio: %s\n",
                    Pa_GetErrorText(err));
    }

    void free(){
        ps_endpointer_free(ep);
        ps_free(decoder);
        ps_config_free(config);
    }
};

/**
 * @brief The BoardMarks enum defiens the type of mark on the board.
 */
enum class BoardMarks { Empty, X, O };

/**
 * @brief The BoardState enum defines the possible states of a board.
 */
enum class BoardState { NoWinner, XWins, OWins, Tie };

#endif // TTTCOMMONTYPES_H
