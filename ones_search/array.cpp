/*
 * Find longest subsequence of 1's in a sequence of 0's and 1's with at most one
 * 0 captured in between 1's. State machine solution.
 */
#include <cassert>

#define ARR_SIZE(seq) (sizeof(seq) / sizeof(seq[0]))

std::size_t find_max_ones_subsequence(int* seq, std::size_t seq_size)
{
    using namespace std;
    size_t result = 0;
    // Out of sequence index (zero was not encountered yet)
    size_t last_zero_position_idx = seq_size;
    size_t current_length_cntr = 0;

    auto is_none_state = [&current_length_cntr, &last_zero_position_idx, &seq_size]// () -> bool
    {
        return current_length_cntr == 0 && last_zero_position_idx == seq_size;
    };
    auto is_sequence_state = [&current_length_cntr, &last_zero_position_idx, &seq_size]
    {
        return current_length_cntr != 0 && last_zero_position_idx == seq_size;
    };
    auto is_possible_sequence_state = [&] // Capture whole scope inside lambda
    {
        return current_length_cntr != 0 && last_zero_position_idx != seq_size;
    };

    for (size_t i = 0; i < seq_size; ++i)
    {
        assert("Can't be 1 and 3 simultaniously" && (
             is_none_state()
          != is_sequence_state()
          != is_possible_sequence_state()
        ));

        // None state
        if (is_none_state())
        {
            if (1 == seq[i])
                // Transit to 'inside sequence' state
                ++current_length_cntr;
            // Back loop transit
            continue;
        }

        // 'Inside sequence' state
        if (is_sequence_state())
        {
            if (0 == seq[i])
                // Transit to 'possible sequence' state
                last_zero_position_idx = i;

            ++current_length_cntr;
            // Back loop transit
            continue;
        }

        // Possibly overlapping sequence state
        if (is_possible_sequence_state())
        {
            if (last_zero_position_idx + 1 == i && 0 == seq[i])
            {
                // Transit to 'none' state
                last_zero_position_idx = seq_size;
                current_length_cntr = 0;
            }
            else if (0 == seq[i])
            {
                if (result < current_length_cntr)
                    result = current_length_cntr;
                // Transit to 'sequence' state
                i = last_zero_position_idx + 1;
                current_length_cntr = 1;
                last_zero_position_idx = seq_size;
            }
            else
                // Backloop transit
                ++current_length_cntr;

            continue;
        }
    } // For loop end

    assert("Can't be 1 and 3 simultaniously" && (
         is_none_state()
      != is_sequence_state()
      != is_possible_sequence_state()
    ));

    if (result < current_length_cntr)
        result = current_length_cntr;

    return result;
}

int main()
{
    using namespace std;
    {
        int seq[] = {};
        assert("Empty arr" && 0 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
    {
        int seq[] = {0, 0, 0};
        assert("Zeroes only" && 0 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
    {
        int seq[] = {1};
        assert("One one" && 1 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
    {
        int seq[] = {1, 0, 1, 0, 1, 1};
        assert("Overlapping seqs" && 4 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
    {
        int seq[] = {1, 1, 1, 1, 1};
        assert("Ones only" && 5 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
    {
        int seq[] = {0, 1, 1, 0, 1, 1, 1};
        assert("Another overlap" && 6 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
    {
        int seq[] = {1, 0, 1, 0, 1, 0, 1};
        assert("Tricky one" && 3 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
    {
        int seq[] = {1, 0, 0, 0, 1, 0, 1, 1};
        assert("Lot's of zeroes" && 4 == find_max_ones_subsequence(seq,  ARR_SIZE(seq)));
    }
}
