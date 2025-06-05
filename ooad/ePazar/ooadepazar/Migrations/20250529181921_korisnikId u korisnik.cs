using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class korisnikIdukorisnik : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikIdId",
                table: "Artikal");

            migrationBuilder.RenameColumn(
                name: "KorisnikIdId",
                table: "Artikal",
                newName: "KorisnikId");

            migrationBuilder.RenameIndex(
                name: "IX_Artikal_KorisnikIdId",
                table: "Artikal",
                newName: "IX_Artikal_KorisnikId");

            migrationBuilder.AddForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikId",
                table: "Artikal",
                column: "KorisnikId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikId",
                table: "Artikal");

            migrationBuilder.RenameColumn(
                name: "KorisnikId",
                table: "Artikal",
                newName: "KorisnikIdId");

            migrationBuilder.RenameIndex(
                name: "IX_Artikal_KorisnikId",
                table: "Artikal",
                newName: "IX_Artikal_KorisnikIdId");

            migrationBuilder.AddForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikIdId",
                table: "Artikal",
                column: "KorisnikIdId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }
    }
}
