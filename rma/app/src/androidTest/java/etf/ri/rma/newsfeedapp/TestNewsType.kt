package etf.ri.rma.newsfeedapp

import androidx.compose.ui.test.getBoundsInRoot
import androidx.compose.ui.test.junit4.createComposeRule
import androidx.compose.ui.test.onNodeWithContentDescription
import androidx.compose.ui.test.onNodeWithText
import androidx.test.ext.junit.runners.AndroidJUnit4
import etf.ri.rma.newsfeedapp.data.NewsData
import etf.ri.rma.newsfeedapp.screen.FeaturedNewsCard
import etf.ri.rma.newsfeedapp.screen.StandardNewsCard
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import kotlin.test.fail

@RunWith(AndroidJUnit4::class)
class TestNewsType {
    @get:Rule
    val composeTestRule = createComposeRule()
    @Test
    fun checkFeatured(){
        val featured = NewsData.getAllNews().find { it.isFeatured }
        if(featured!=null){
            composeTestRule.setContent { FeaturedNewsCard(featured) }
            val image = composeTestRule.onNodeWithContentDescription("image", ignoreCase = true, substring = true)
            val imageBounds = image.getBoundsInRoot()
            val title = composeTestRule.onNodeWithText(featured.title)
            val titleBounds = title.getBoundsInRoot()
            assert(imageBounds.left==titleBounds.left)
        }else {
            fail("Nema featured vijesti!")
        }
    }
    @Test
    fun checkStandard(){
        val nonfeatured = NewsData.getAllNews().find { !it.isFeatured }
        if(nonfeatured!=null){
            composeTestRule.setContent { StandardNewsCard(nonfeatured) }
            val image = composeTestRule.onNodeWithContentDescription("image", ignoreCase = true, substring = true)
            val imageBounds = image.getBoundsInRoot()
            val title = composeTestRule.onNodeWithText(nonfeatured.title)
            val titleBounds = title.getBoundsInRoot()
            assert(imageBounds.right<=titleBounds.left)
        }else {
            fail("Nema featured vijesti!")
        }
    }
}