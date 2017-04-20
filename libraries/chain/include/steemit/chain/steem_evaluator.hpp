#pragma once

#include <steemit/protocol/steem_operations.hpp>

#include <steemit/chain/evaluator.hpp>

namespace steemit {
    namespace chain {

        class account_create_evaluator
                : public steemit::chain::evaluator_impl<account_create_evaluator> {
        public:
            typedef protocol::account_create_operation operation_type;

            account_create_evaluator(database &db)
                    : steemit::chain::evaluator_impl<account_create_evaluator>(db) {
            }

            void do_apply(const protocol::account_create_operation &o);
        };

        class account_update_evaluator
                : public steemit::chain::evaluator_impl<account_update_evaluator> {
        public:
            typedef protocol::account_update_operation operation_type;

            account_update_evaluator(database &db)
                    : steemit::chain::evaluator_impl<account_update_evaluator>(db) {
            }

            void do_apply(const protocol::account_update_operation &o);
        };

        class transfer_evaluator
                : public steemit::chain::evaluator_impl<transfer_evaluator> {
        public:
            typedef protocol::transfer_operation operation_type;

            transfer_evaluator(database &db)
                    : steemit::chain::evaluator_impl<transfer_evaluator>(db) {
            }

            void do_apply(const protocol::transfer_operation &o);
        };

        class transfer_to_vesting_evaluator
                : public steemit::chain::evaluator_impl<transfer_to_vesting_evaluator> {
        public:
            typedef protocol::transfer_to_vesting_operation operation_type;

            transfer_to_vesting_evaluator(database &db)
                    : steemit::chain::evaluator_impl<transfer_to_vesting_evaluator>(db) {
            }

            void do_apply(const protocol::transfer_to_vesting_operation &o);
        };

        class witness_update_evaluator
                : public steemit::chain::evaluator_impl<witness_update_evaluator> {
        public:
            typedef protocol::witness_update_operation operation_type;

            witness_update_evaluator(database &db)
                    : steemit::chain::evaluator_impl<witness_update_evaluator>(db) {
            }

            void do_apply(const protocol::witness_update_operation &o);
        };

        class account_witness_vote_evaluator
                : public steemit::chain::evaluator_impl<account_witness_vote_evaluator> {
        public:
            typedef protocol::account_witness_vote_operation operation_type;

            account_witness_vote_evaluator(database &db)
                    : steemit::chain::evaluator_impl<account_witness_vote_evaluator>(db) {
            }

            void do_apply(const protocol::account_witness_vote_operation &o);
        };

        class account_witness_proxy_evaluator
                : public steemit::chain::evaluator_impl<account_witness_proxy_evaluator> {
        public:
            typedef protocol::account_witness_proxy_operation operation_type;

            account_witness_proxy_evaluator(database &db)
                    : steemit::chain::evaluator_impl<account_witness_proxy_evaluator>(db) {
            }

            void do_apply(const protocol::account_witness_proxy_operation &o);
        };

        class withdraw_vesting_evaluator
                : public steemit::chain::evaluator_impl<withdraw_vesting_evaluator> {
        public:
            typedef protocol::withdraw_vesting_operation operation_type;

            withdraw_vesting_evaluator(database &db)
                    : steemit::chain::evaluator_impl<withdraw_vesting_evaluator>(db) {
            }

            void do_apply(const protocol::withdraw_vesting_operation &o);
        };

        class set_withdraw_vesting_route_evaluator
                : public steemit::chain::evaluator_impl<set_withdraw_vesting_route_evaluator> {
        public:
            typedef protocol::set_withdraw_vesting_route_operation operation_type;

            set_withdraw_vesting_route_evaluator(database &db)
                    : steemit::chain::evaluator_impl<set_withdraw_vesting_route_evaluator>(db) {
            }

            void do_apply(const protocol::set_withdraw_vesting_route_operation &o);
        };

        class comment_evaluator
                : public steemit::chain::evaluator_impl<comment_evaluator> {
        public:
            typedef protocol::comment_operation operation_type;

            comment_evaluator(database &db)
                    : steemit::chain::evaluator_impl<comment_evaluator>(db) {
            }

            void do_apply(const protocol::comment_operation &o);
        };

        class comment_options_evaluator
                : public steemit::chain::evaluator_impl<comment_options_evaluator> {
        public:
            typedef protocol::comment_options_operation operation_type;

            comment_options_evaluator(database &db)
                    : steemit::chain::evaluator_impl<comment_options_evaluator>(db) {
            }

            void do_apply(const protocol::comment_options_operation &o);
        };

        template<typename PriceEvaluator>
        class comment_payout_extension_evaluator
                : public steemit::chain::evaluator_impl<comment_payout_extension_evaluator<PriceEvaluator>> {
        public:
            typedef protocol::comment_payout_extension_operation operation_type;
            typedef PriceEvaluator price_evaluator_type;

            comment_payout_extension_evaluator(database &db)
                    : steemit::chain::evaluator_impl<comment_payout_extension_evaluator>(db) {
            }

            void do_apply(const protocol::comment_payout_extension_operation &o);
        };

        STEEMIT_DEFINE_EVALUATOR(delete_comment)

        STEEMIT_DEFINE_EVALUATOR(vote)

        STEEMIT_DEFINE_EVALUATOR(custom)

        STEEMIT_DEFINE_EVALUATOR(custom_json)

        STEEMIT_DEFINE_EVALUATOR(custom_binary)

        STEEMIT_DEFINE_EVALUATOR(pow)

        STEEMIT_DEFINE_EVALUATOR(pow2)

        STEEMIT_DEFINE_EVALUATOR(feed_publish)

        STEEMIT_DEFINE_EVALUATOR(convert)

        STEEMIT_DEFINE_EVALUATOR(limit_order_create)

        STEEMIT_DEFINE_EVALUATOR(limit_order_cancel)

        STEEMIT_DEFINE_EVALUATOR(report_over_production)

        STEEMIT_DEFINE_EVALUATOR(limit_order_create2)

        STEEMIT_DEFINE_EVALUATOR(escrow_transfer)

        STEEMIT_DEFINE_EVALUATOR(escrow_approve)

        STEEMIT_DEFINE_EVALUATOR(escrow_dispute)

        STEEMIT_DEFINE_EVALUATOR(escrow_release)

        STEEMIT_DEFINE_EVALUATOR(challenge_authority)

        STEEMIT_DEFINE_EVALUATOR(prove_authority)

        STEEMIT_DEFINE_EVALUATOR(request_account_recovery)

        STEEMIT_DEFINE_EVALUATOR(recover_account)

        STEEMIT_DEFINE_EVALUATOR(change_recovery_account)

        STEEMIT_DEFINE_EVALUATOR(transfer_to_savings)

        STEEMIT_DEFINE_EVALUATOR(transfer_from_savings)

        STEEMIT_DEFINE_EVALUATOR(cancel_transfer_from_savings)

        STEEMIT_DEFINE_EVALUATOR(decline_voting_rights)

        STEEMIT_DEFINE_EVALUATOR(reset_account)

        STEEMIT_DEFINE_EVALUATOR(set_reset_account)
    }
} // steemit::chain
