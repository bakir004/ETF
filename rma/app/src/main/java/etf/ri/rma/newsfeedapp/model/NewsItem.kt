package etf.ri.rma.newsfeedapp.model

data class NewsItem(
    val id: String, // koristite kao key za lazylistu i neka bude jedinstveno za svaku vijest
    val title: String,
    val snippet: String,
    val imageUrl: String?, // URL slike - ne koristi se sada
    val category: String, // primjer "Politika", "Sport"
    val isFeatured: Boolean, // Tip novosti “Featured” ili “Non featured”
    val source: String,
    val publishedDate: String
)
