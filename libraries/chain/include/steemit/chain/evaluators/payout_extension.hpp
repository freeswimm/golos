#ifndef STEEM_PAYOUT_EXTENSION_HPP
#define STEEM_PAYOUT_EXTENSION_HPP

#include <steemit/chain/database.hpp>

namespace steemit {
    namespace chain {
        class comment_object;

        namespace evaluators {
            template<typename TimeTypename, typename AssetTypename>
            class base_comment_payout_extension {
            public:
                virtual TimeTypename time_by_cost(const asset &input_asset, const chain::comment_object &input_comment) const = 0;

                virtual AssetTypename cost_by_time(const time_point_sec &input_time, const chain::comment_object &input_comment) const = 0;

            };

            class comment_payout_extension
                    : public base_comment_payout_extension<fc::time_point_sec, asset> {
            public:
                virtual fc::time_point_sec time_by_cost(const asset &input_asset, const chain::comment_object &input_comment) const override;

                virtual asset cost_by_time(const fc::time_point_sec &input_time, const chain::comment_object &input_comment) const override;

                const static share_type cost_per_day;
            };

            const share_type comment_payout_extension::cost_per_day = 10;
        }
    }
}

#endif //STEEM_PAYOUT_EXTENSION_HPP