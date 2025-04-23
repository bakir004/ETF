package etf.ri.rma.newsfeedapp

import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.assertIsSelected
import androidx.compose.ui.test.hasClickAction
import androidx.compose.ui.test.hasTestTag
import androidx.compose.ui.test.hasText
import androidx.compose.ui.test.junit4.createComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.compose.ui.test.performScrollToNode
import androidx.test.ext.junit.runners.AndroidJUnit4
import etf.ri.rma.newsfeedapp.data.NewsData
import etf.ri.rma.newsfeedapp.screen.NewsFeedScreen
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith
import kotlin.test.assertFailsWith
import kotlin.test.assertTrue

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class TestS1 {

    @get:Rule
    val composeTestRule = createComposeRule()
    @Before
    fun setContentForTests() {
        assertTrue (message = "Nemate dovoljno vijesti!", actual = NewsData.getAllNews().count()>=20)
        composeTestRule.setContent { NewsFeedScreen() }
    }


    @Test
    fun allNews() {
        var novosti = NewsData.getAllNews()
        for(novost in novosti){
            val listnode = composeTestRule.onNodeWithTag("news_list")
            listnode.performScrollToNode(hasText(novost.title))
        }
    }
    @Test
    fun filtersVisible(){
        var listaFilter = listOf("spo","all","sci","pol")
        for(filter in listaFilter){
            composeTestRule.onNode(hasTestTag("filter_chip_$filter") and hasClickAction()).assertIsDisplayed().performClick()
            composeTestRule.onNode(hasTestTag("filter_chip_$filter") and hasClickAction()).assertIsSelected()
        }

    }
    @Test
    fun chipSport() {
        composeTestRule.onNode(hasTestTag("filter_chip_spo") and hasClickAction()).assertIsDisplayed().performClick()
        composeTestRule.onNode(hasTestTag("filter_chip_spo") and hasClickAction()).assertIsSelected()
        var novosti = NewsData.getAllNews()
        val listnode = composeTestRule.onNodeWithTag("news_list")
        for(novost in novosti){
            if(novost.category.contains("spo",ignoreCase = true)){
                listnode.performScrollToNode(hasText(novost.title))
                composeTestRule.onNodeWithText(novost.title).assertIsDisplayed()
            }
            else
                assertFailsWith<AssertionError>{listnode.performScrollToNode(hasText(novost.title))}
        }
    }

    @Test
    fun chipSport_All() {
        composeTestRule.onNode(hasTestTag("filter_chip_spo") and hasClickAction()).assertIsDisplayed().performClick()
        composeTestRule.onNode(hasTestTag("filter_chip_spo") and hasClickAction()).assertIsSelected()
        composeTestRule.onNode(hasTestTag("filter_chip_all") and hasClickAction()).assertIsDisplayed().performClick()
        composeTestRule.onNode(hasTestTag("filter_chip_all") and hasClickAction()).assertIsSelected()
        var novosti = NewsData.getAllNews()
        val listnode = composeTestRule.onNodeWithTag("news_list")
        for(novost in novosti){
            listnode.performScrollToNode(hasText(novost.title))
            composeTestRule.onNodeWithText(novost.title).assertIsDisplayed()
        }
    }
    @Test
    fun listItemHasAll(){
        composeTestRule.onNode(hasTestTag("filter_chip_all") and hasClickAction()).assertIsDisplayed().performClick()
        composeTestRule.onNode(hasTestTag("filter_chip_all") and hasClickAction()).assertIsSelected()
        val novost = NewsData.getAllNews()[0]
        val listnode = composeTestRule.onNodeWithTag("news_list")
        listnode.performScrollToNode(hasText(novost.title))
        composeTestRule.onNodeWithText(novost.title.substring(0..15),substring = true).assertIsDisplayed()
        composeTestRule.onNodeWithText(novost.snippet.substring(0..15),substring = true).assertIsDisplayed()
        composeTestRule.onNodeWithText(novost.source,substring = true).assertIsDisplayed()
    }
}