#pragma once
#include "stone.h"
#include <vector>
#include <limits>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace F19 {

//	石の並びを表現するクラス
	class StoneLine : public std::vector<Stone> {
	public:

	//	ハッシュ関数用のクラス
		struct Hash;

	//	基底型(石の配列)
		using Base = std::vector<Stone>;

	//	このクラス用のポインタ
		using Ptr = std::shared_ptr<StoneLine>;

	//	このクラスのインスタンスを保持するためのマップ
		using Map = std::unordered_map<Base, Ptr, Hash>;

		using Steps = int;
		constexpr static Steps Steps_Infinity = 99; // = std::numeric_limits<Steps>::max();

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

	//	各位置の猶予ステップ数(その位置にtargetの石を置いた時の猶予ステップ数)
		Steps grace_steps(Stone target, size_t index) const;

	//	等価比較(ハッシュマップで利用)
		bool operator==(const StoneLine&) const;
		bool operator!=(const StoneLine&) const;

	//	デバッグ用
		static void out_instances(std::ostream& os);

	private:
		StoneLine(const Base&);

	//	win_steps を計算する関数
		Steps calc_win_steps(Stone target) const;

	//	blocked_steps を計算する関数
		Steps calc_blocked_steps(Stone target) const;

	//	各位置における grace_steps を計算する関数
		void calc_grace_steps(Stone target) const;


		void out_graces(Stone target) const;


		mutable std::unique_ptr<Steps> _win_steps[2];
		mutable std::unique_ptr<Steps> _blocked_steps[2];
		mutable std::vector<Steps> _grace_steps[2];

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