#pragma once
#include "stone.h"
#include <vector>
#include <limits>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace F19 {

//	石の列（並び）を表現するクラス
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

		using Index = size_t;

	//	評価値の型
		using Value = double;

	//	既に作成されたインスタンスを保存するマップ
		static Map instances;

	//	石の並びからインスタンスを取得する（ない場合は作成する）
		static Ptr instance(const Base&);

	//	位置 index (0から数える) に石 stone を置いた列を取得
		Ptr getWith(Index index, Stone stone) const;

	//	石を反転させた列を取得
		Ptr getReversed() const;

	//	targetの石が5目(5連続)を形成しているかどうか
		bool is5mk(Stone target) const;

	//	targetの石についての評価値を取得
		Value value(Stone target) const;

	//	位置 index に石 target を置いた時の評価値を取得
		Value next_value(Stone target, Index index) const;

	////	targetの石を位置indexに置いた時の評価値から現在の評価値を引いた値
	//	Value next_value_diff(Stone target, Index index) const;

	//	等価比較(ハッシュマップで利用)
		bool operator==(const StoneLine&) const;
		
	//	不等価比較(ハッシュマップで利用)
		bool operator!=(const StoneLine&) const;

	//	デバッグ用
		static void out_instances(std::ostream& os);

	//	無視係数
		static double miss_coef;

	private:
	//	コンストラクタ（石の配列から StoneLine オブジェクトを作成）
		StoneLine(const Base&);

	//	すべての次の評価値を計算する関数
	//	void calc_next_values(Stone target) const;

		mutable std::unique_ptr<Value> _value[2];
		mutable std::vector<Value> _next_values[2];

	};
		
//	入力関数
	std::istream& operator >>(std::istream& is, StoneLine& stone_line);
	
//	出力関数
	std::ostream& operator <<(std::ostream& os, const StoneLine& stone_line);

//	ハッシュ関数用のクラス
	struct StoneLine::Hash {
		using result_type = size_t;
	//	ハッシュ関数
		size_t operator()(const Base& stone_line) const;
	};

}