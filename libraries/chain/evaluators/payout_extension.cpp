#include <steemit/chain/evaluators/payout_extension.hpp>
#include <steemit/chain/comment_object.hpp>

namespace steemit {
    namespace chain {
        namespace evaluators {
            fc::time_point_sec comment_payout_extension::time_by_cost(const asset &input_asset, const chain::comment_object &input_comment) const {
                FC_ASSERT(input_asset.symbol ==
                          SBD_SYMBOL, "Extension payment should be in SBD");
                FC_ASSERT(input_asset.amount / cost_per_day >
                          0, "Extension payment should cover more than a day");
                return fc::time_point_sec(static_cast<uint32_t>((
                        (input_asset.amount * 60 * 60 * 24 *
                         input_comment.net_rshares) / (cost_per_day *
                                                       input_comment.cashout_windows_amount)).value));
            }

            asset comment_payout_extension::cost_by_time(const fc::time_point_sec &input_time, const chain::comment_object &input_comment) const {
                FC_ASSERT(input_time.sec_since_epoch() / (60 * 60 * 24) >
                          0, "Extension time should be equal or greater than a day");
                FC_ASSERT(input_time.sec_since_epoch() <
                          STEEMIT_CASHOUT_WINDOW_SECONDS, "Extension time should be less or equal than a week");

                return asset((input_time.sec_since_epoch() * cost_per_day *
                              input_comment.cashout_windows_amount) /
                             (input_comment.net_rshares * 60 * 60 *
                              24), SBD_SYMBOL);
            }
        }
    }
}