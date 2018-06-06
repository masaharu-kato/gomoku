#include "base.h"
#include "stone.h"
#include "position.h"
#include <vector>
#include <memory>
#include <functional>

namespace Kato {

	class Board;
	using BoardPtr = std::shared_ptr<Board>;

	class Board {
		using Data = Stone::Kind**;
		using History = std::vector<Position>;
		using Count = int;
		constexpr static int Size = BOARD_SIZE;
		constexpr static int SizeX = Size;
		constexpr static int SizeY = Size;

		Data data;
		History history;
		Count count;

		

	/*
		struct Data {
			Position p;
			Stone::Kind kind;
			Stone::Series series;

			Data(const Position& p, Stone::Kind kind) : p(p), kind(kind) {}
		};

		std::vector<Data> series_list;

	//	ある位置に関して処理を行う
		void calcOn(const Position& pos);

	//	位置と方向を指定して連続数を計算
		void calcSeries(Stone::Series::Dir&, Position pos, Direction dir);

	//	位置の全方向の連続数を計算
		void calcSeries(const Position& pos);
	*/

	//	位置が範囲内か返す
		bool isInside(const Position& pos) const;

	//	データ上の指定した位置に石を置く(履歴, 手番のカウントは行わない)
		Stone::Kind setStone(const Position& pos);

	public:

		Board() {};

	//	指定位置の石の種類を返す　範囲外なら-1
		Stone::Kind getStone(const Position&) const;

	//	指定位置に任意の色の石を置く
		Stone::Kind putStone(const Position&, Stone::Kind&);

		const History& getHistory() const {
			return history;
		}

	//	n番目に置いた石の色を取得 (初手を 0 とする)
		Position getHistory(Count) const;

	//	最後からn番目に置いた石の色を取得 (一つ前の手を 1 とする)
		Position getLastHistory(Count) const;

	//	現在何手目であるか取得する
		Count getCount() const;

	//	現在のターンを取得する
		Stone::Kind getKind() const;
		
	//	盤面上のすべての位置に関して処理を行う
		void doEach(std::function<void(const Position&)>) const;
		void doEach(std::function<void(const Position&, Stone::Kind)>) const;
		void doEach(std::function<void(const Position&, Stone::Kind&)>);
		void doEachEmpty(std::function<void(const Position&)>) const;
		Stone::Kind& operator [](const Position&);

	};

}