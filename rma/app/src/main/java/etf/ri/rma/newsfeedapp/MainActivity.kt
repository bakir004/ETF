package etf.ri.rma.newsfeedapp

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.material3.Scaffold
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.ui.Modifier
import etf.ri.rma.newsfeedapp.screen.NewsFeedScreen
import etf.ri.rma.newsfeedapp.ui.theme.ProjekatTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            ProjekatTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    NewsFeedScreen()
                }
            }
        }
    }
}

