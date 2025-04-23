package etf.ri.rma.newsfeedapp.screen

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.unit.dp
import etf.ri.rma.newsfeedapp.model.NewsItem

@Composable
fun NewsList(
    newsList: List<NewsItem>
) {
    LazyVerticalGrid(
        columns = GridCells.Fixed(1),
        verticalArrangement = Arrangement.spacedBy(6.dp),
        modifier = Modifier
            .fillMaxSize()
            .testTag("news_list")
    ) {
        items(newsList) { news ->
            if(news.isFeatured) {
                FeaturedNewsCard(news)
            } else {
                StandardNewsCard(news)
            }
        }
    }
}
