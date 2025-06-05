using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ooadepazar.Models
{
    public class Pracenje
    {
        [Key]
        public int ID { get; set; }

        public ApplicationUser PraceniID { get; set; }
        public ApplicationUser PratilacID { get; set; }
    }
}
