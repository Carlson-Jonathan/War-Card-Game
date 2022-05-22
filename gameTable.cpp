#include "gameTable.h"

void GameTable::activateGameRound() {
    timeElapsed = clock.getElapsedTime().asMilliseconds();

    if(mayStartNewRound) revealFaceUpCardsWithDelayOf(placementDelay);
    if(allPlayersPlayedACard && hiPlayersNotYetDetermined) determineWinnerOrTie();
    if(mayDeclareRoundResults && timeElapsed > resultsDelay) declareRoundResults();
    if(mayBreakTie) breakTie();
    if(mayConcludeRound && timeElapsed > conclusionDelay) concludeRound();
}
