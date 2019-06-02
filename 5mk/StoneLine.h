#pragma once
#include "stone.h"
#include <vector>
#include <limits>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace F19 {

	class StoneLine : public std::vector<Stone> {
	public:
		struct Hash;

		using Base = std::vector<Stone>;
		using Ptr = std::shared_ptr<StoneLine>;
		using Map = std::unordered_map<Base, Ptr, Hash>;

		using Steps = size_t;
		constexpr static Steps Steps_Infinity = 99; // = std::numeric_limits<size_t>::max();

		static Map instances;
		static Ptr instance(const Base&);

		Ptr getWith(size_t index, Stone stone) const;
		Ptr getReversed() const;

	//	targetの石が5目(5連続)を形成しているかどうか
		bool is5mk(Stone target) const;

	//	targetの石が勝利するまでの最小ステップ数
		Steps win_steps(Stone target) const;

	//	targetの石の勝利が阻止される(targetの石が5目を作れなくなる)までの最小ステップ数
		Steps blocked_steps(Stone target) const;
		
	//	猶予ステップ数
		Steps grace_steps(Stone target) const;

	//	等価比較(ハッシュマップで利用)
		bool operator==(const StoneLine&) const;
		bool operator!=(const StoneLine&) const;

	//	デバッグ用
		static void out_instances(std::ostream& os);

	private:
		StoneLine(const Base&);

	//	win_steps の計算関数
		Steps calc_win_steps(Stone target) const;

	//	blocked_steps の計算関数
		Steps calc_blocked_steps(Stone target) const;

	//	計算済みかのフラグ

		mutable std::unique_ptr<Steps> _win_steps[2];
		mutable std::unique_ptr<Steps> _blocked_steps[2];

	};
		
//	出力関数
	std::istream& operator >>(std::istream& is, StoneLine& stone_line);

//	入力関数
	std::ostream& operator <<(std::ostream& os, const StoneLine& stone_line);

//	ハッシュ関数
	struct StoneLine::Hash {
		using result_type = size_t;
		size_t operator()(const Base& stone_line) const;
	};

}